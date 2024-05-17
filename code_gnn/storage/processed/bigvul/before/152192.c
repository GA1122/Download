void RenderFrameImpl::BeginNavigationInternal(
    std::unique_ptr<blink::WebNavigationInfo> info) {
  std::unique_ptr<DocumentState> document_state_owned = BuildDocumentState();
  DocumentState* document_state = document_state_owned.get();
  if (!frame_->CreatePlaceholderDocumentLoader(
          *info, std::move(document_state_owned))) {
    return;
  }

  browser_side_navigation_pending_ = true;
  browser_side_navigation_pending_url_ = info->url_request.Url();

  blink::WebURLRequest& request = info->url_request;

  WebDocument frame_document = frame_->GetDocument();
  if (info->frame_type == network::mojom::RequestContextFrameType::kTopLevel)
    request.SetSiteForCookies(request.Url());
  else
    request.SetSiteForCookies(frame_document.SiteForCookies());

  ui::PageTransition transition_type = GetTransitionType(
      ui::PAGE_TRANSITION_LINK,
      info->frame_load_type == WebFrameLoadType::kReplaceCurrentItem,
      IsMainFrame(), info->navigation_type);
  if (info->is_client_redirect) {
    transition_type = ui::PageTransitionFromInt(
        transition_type | ui::PAGE_TRANSITION_CLIENT_REDIRECT);
  }

  WillSendRequestInternal(
      request,
      frame_->Parent() ? ResourceType::kSubFrame : ResourceType::kMainFrame,
      document_state, transition_type);

  if (!info->url_request.GetExtraData())
    info->url_request.SetExtraData(std::make_unique<RequestExtraData>());

  DCHECK_EQ(network::mojom::FetchRequestMode::kNavigate,
            info->url_request.GetFetchRequestMode());
  DCHECK_EQ(network::mojom::FetchCredentialsMode::kInclude,
            info->url_request.GetFetchCredentialsMode());
  DCHECK_EQ(network::mojom::FetchRedirectMode::kManual,
            info->url_request.GetFetchRedirectMode());
  DCHECK(frame_->Parent() ||
         info->frame_type ==
             network::mojom::RequestContextFrameType::kTopLevel);
  DCHECK(!frame_->Parent() ||
         info->frame_type == network::mojom::RequestContextFrameType::kNested);

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

  int load_flags = info->url_request.GetLoadFlagsForWebUrlRequest();
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
    navigation_client_impl_->MarkWasInitiatedInThisFrame();
  }

  blink::mojom::NavigationInitiatorPtr initiator_ptr(
      blink::mojom::NavigationInitiatorPtrInfo(
          std::move(info->navigation_initiator_handle), 0));

  bool current_frame_has_download_sandbox_flag =
      !frame_->IsAllowedToDownloadWithoutUserActivation();
  bool has_download_sandbox_flag =
      info->initiator_frame_has_download_sandbox_flag ||
      current_frame_has_download_sandbox_flag;
  bool from_ad = info->initiator_frame_is_ad || frame_->IsAdSubframe();

  GetFrameHost()->BeginNavigation(
      MakeCommonNavigationParams(frame_->GetSecurityOrigin(), std::move(info),
                                 load_flags, has_download_sandbox_flag,
                                 from_ad),
      std::move(begin_navigation_params), std::move(blob_url_token),
      std::move(navigation_client_info), std::move(initiator_ptr));
}
