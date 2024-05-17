void RenderFrameImpl::BeginNavigationInternal(
    std::unique_ptr<blink::WebNavigationInfo> info) {
  if (!CreatePlaceholderDocumentLoader(*info))
    return;

  WebDocumentLoader* document_loader = frame_->GetProvisionalDocumentLoader();
  NavigationState* navigation_state =
      NavigationState::FromDocumentLoader(document_loader);

  browser_side_navigation_pending_ = true;
  browser_side_navigation_pending_url_ = info->url_request.Url();

  blink::WebURLRequest& request = info->url_request;

  WebDocument frame_document = frame_->GetDocument();
  if (request.GetFrameType() ==
      network::mojom::RequestContextFrameType::kTopLevel)
    request.SetSiteForCookies(request.Url());
  else
    request.SetSiteForCookies(frame_document.SiteForCookies());

  WillSendRequest(request);

  if (!info->url_request.GetExtraData())
    info->url_request.SetExtraData(std::make_unique<RequestExtraData>());
  if (info->is_client_redirect) {
    RequestExtraData* extra_data =
        static_cast<RequestExtraData*>(info->url_request.GetExtraData());
    extra_data->set_transition_type(ui::PageTransitionFromInt(
        extra_data->transition_type() | ui::PAGE_TRANSITION_CLIENT_REDIRECT));
  }

  DCHECK_EQ(network::mojom::FetchRequestMode::kNavigate,
            info->url_request.GetFetchRequestMode());
  DCHECK_EQ(network::mojom::FetchCredentialsMode::kInclude,
            info->url_request.GetFetchCredentialsMode());
  DCHECK_EQ(network::mojom::FetchRedirectMode::kManual,
            info->url_request.GetFetchRedirectMode());
  DCHECK(frame_->Parent() ||
         info->url_request.GetFrameType() ==
             network::mojom::RequestContextFrameType::kTopLevel);
  DCHECK(!frame_->Parent() ||
         info->url_request.GetFrameType() ==
             network::mojom::RequestContextFrameType::kNested);

  bool is_form_submission =
      info->navigation_type == blink::kWebNavigationTypeFormSubmitted ||
      info->navigation_type == blink::kWebNavigationTypeFormResubmitted;

  GURL searchable_form_url;
  std::string searchable_form_encoding;
  if (!info->form.IsNull()) {
    WebSearchableFormData web_searchable_form_data(info->form);
    searchable_form_url = web_searchable_form_data.Url();
    searchable_form_encoding = web_searchable_form_data.Encoding().Utf8();
  }

  GURL client_side_redirect_url;
  if (info->is_client_redirect)
    client_side_redirect_url = frame_->GetDocument().Url();

  blink::mojom::BlobURLTokenPtr blob_url_token(
      CloneBlobURLToken(info->blob_url_token.get()));

  int load_flags = GetLoadFlagsForWebURLRequest(info->url_request);
  std::unique_ptr<base::DictionaryValue> initiator =
      GetDevToolsInitiator(info->devtools_initiator_info);
  mojom::BeginNavigationParamsPtr begin_navigation_params =
      mojom::BeginNavigationParams::New(
          GetWebURLRequestHeadersAsString(info->url_request), load_flags,
          info->url_request.GetSkipServiceWorker(),
          GetRequestContextTypeForWebURLRequest(info->url_request),
          GetMixedContentContextTypeForWebURLRequest(info->url_request),
          is_form_submission, searchable_form_url, searchable_form_encoding,
          client_side_redirect_url,
          initiator ? base::make_optional<base::Value>(std::move(*initiator))
                    : base::nullopt);

  mojom::NavigationClientAssociatedPtrInfo navigation_client_info;
  if (IsPerNavigationMojoInterfaceEnabled()) {
    BindNavigationClient(mojo::MakeRequest(&navigation_client_info));
    navigation_state->set_navigation_client(std::move(navigation_client_impl_));
  }

  blink::mojom::NavigationInitiatorPtr initiator_ptr(
      blink::mojom::NavigationInitiatorPtrInfo(
          std::move(info->navigation_initiator_handle), 0));

  bool prevent_sandboxed_download =
      (frame_->EffectiveSandboxFlags() & blink::WebSandboxFlags::kDownloads) !=
          blink::WebSandboxFlags::kNone &&
      info->blocking_downloads_in_sandbox_enabled;

  GetFrameHost()->BeginNavigation(
      MakeCommonNavigationParams(frame_->GetSecurityOrigin(), std::move(info),
                                 load_flags, prevent_sandboxed_download),
      std::move(begin_navigation_params), std::move(blob_url_token),
      std::move(navigation_client_info), std::move(initiator_ptr));

  DCHECK(navigation_state->IsContentInitiated());
  for (auto& observer : observers_) {
    observer.DidStartProvisionalLoad(document_loader,
                                     true  );
  }
}
