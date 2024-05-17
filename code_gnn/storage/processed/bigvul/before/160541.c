void RenderFrameImpl::BeginNavigation(const NavigationPolicyInfo& info) {
  browser_side_navigation_pending_ = true;
  browser_side_navigation_pending_url_ = info.url_request.Url();

  blink::WebURLRequest& request = info.url_request;

  WebDocument frame_document = frame_->GetDocument();
  if (request.GetFrameType() ==
      network::mojom::RequestContextFrameType::kTopLevel)
    request.SetSiteForCookies(request.Url());
  else
    request.SetSiteForCookies(frame_document.SiteForCookies());

  WillSendRequest(request);

  if (!info.url_request.GetExtraData())
    info.url_request.SetExtraData(new RequestExtraData());
  if (info.is_client_redirect) {
    RequestExtraData* extra_data =
        static_cast<RequestExtraData*>(info.url_request.GetExtraData());
    extra_data->set_transition_type(ui::PageTransitionFromInt(
        extra_data->transition_type() | ui::PAGE_TRANSITION_CLIENT_REDIRECT));
  }

  DCHECK_EQ(network::mojom::FetchRequestMode::kNavigate,
            info.url_request.GetFetchRequestMode());
  DCHECK_EQ(network::mojom::FetchCredentialsMode::kInclude,
            info.url_request.GetFetchCredentialsMode());
  DCHECK_EQ(network::mojom::FetchRedirectMode::kManual,
            info.url_request.GetFetchRedirectMode());
  DCHECK(frame_->Parent() ||
         info.url_request.GetFrameType() ==
             network::mojom::RequestContextFrameType::kTopLevel);
  DCHECK(!frame_->Parent() ||
         info.url_request.GetFrameType() ==
             network::mojom::RequestContextFrameType::kNested);

  DCHECK(!info.url_request.RequestorOrigin().IsNull());
  base::Optional<url::Origin> initiator_origin =
      base::Optional<url::Origin>(info.url_request.RequestorOrigin());

  bool is_form_submission =
      info.navigation_type == blink::kWebNavigationTypeFormSubmitted ||
      info.navigation_type == blink::kWebNavigationTypeFormResubmitted;

  GURL searchable_form_url;
  std::string searchable_form_encoding;
  if (!info.form.IsNull()) {
    WebSearchableFormData web_searchable_form_data(info.form);
    searchable_form_url = web_searchable_form_data.Url();
    searchable_form_encoding = web_searchable_form_data.Encoding().Utf8();
  }

  GURL client_side_redirect_url;
  if (info.is_client_redirect)
    client_side_redirect_url = frame_->GetDocument().Url();

  int load_flags = GetLoadFlagsForWebURLRequest(info.url_request);
  mojom::BeginNavigationParamsPtr begin_navigation_params =
      mojom::BeginNavigationParams::New(
          GetWebURLRequestHeadersAsString(info.url_request), load_flags,
          info.url_request.GetServiceWorkerMode() !=
              blink::WebURLRequest::ServiceWorkerMode::kAll,
          GetRequestContextTypeForWebURLRequest(info.url_request),
          GetMixedContentContextTypeForWebURLRequest(info.url_request),
          is_form_submission, searchable_form_url, searchable_form_encoding,
          initiator_origin, client_side_redirect_url);

  GetFrameHost()->BeginNavigation(MakeCommonNavigationParams(info, load_flags),
                                  std::move(begin_navigation_params));
}
