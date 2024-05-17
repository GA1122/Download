WebURLRequest CreateURLRequestForNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    std::unique_ptr<NavigationResponseOverrideParameters> response_override,
    bool is_view_source_mode_enabled) {
  const GURL navigation_url = !request_params.original_url.is_empty()
                                  ? request_params.original_url
                                  : common_params.url;
  const std::string navigation_method = !request_params.original_method.empty()
                                            ? request_params.original_method
                                            : common_params.method;
  WebURLRequest request(navigation_url);
  request.SetHTTPMethod(WebString::FromUTF8(navigation_method));

  if (is_view_source_mode_enabled)
    request.SetCacheMode(blink::mojom::FetchCacheMode::kForceCache);

  WebString web_referrer;
  if (common_params.referrer.url.is_valid()) {
    web_referrer = WebSecurityPolicy::GenerateReferrerHeader(
        common_params.referrer.policy, common_params.url,
        WebString::FromUTF8(common_params.referrer.url.spec()));
    request.SetHTTPReferrer(web_referrer, common_params.referrer.policy);
    if (!web_referrer.IsEmpty()) {
      request.SetHTTPOriginIfNeeded(
          WebSecurityOrigin(url::Origin::Create(common_params.referrer.url)));
    }
  }

  if (common_params.post_data) {
    request.SetHTTPBody(GetWebHTTPBodyForRequestBody(*common_params.post_data));
    if (!request_params.post_content_type.empty()) {
      request.AddHTTPHeaderField(
          WebString::FromASCII(net::HttpRequestHeaders::kContentType),
          WebString::FromASCII(request_params.post_content_type));
    }
  }

  if (!web_referrer.IsEmpty() || common_params.referrer.policy !=
                                     network::mojom::ReferrerPolicy::kDefault) {
    request.SetHTTPReferrer(web_referrer, common_params.referrer.policy);
  }

  request.SetPreviewsState(
      static_cast<WebURLRequest::PreviewsState>(common_params.previews_state));

  request.SetOriginPolicy(WebString::FromUTF8(common_params.origin_policy));

  if (common_params.initiator_origin)
    request.SetRequestorOrigin(common_params.initiator_origin.value());

  auto extra_data = std::make_unique<RequestExtraData>();
  extra_data->set_navigation_response_override(std::move(response_override));
  extra_data->set_navigation_initiated_by_renderer(
      request_params.nav_entry_id == 0);
  request.SetExtraData(std::move(extra_data));
  request.SetWasDiscarded(request_params.was_discarded);

  return request;
}
