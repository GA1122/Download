WebURLRequest CreateURLRequestForNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    std::unique_ptr<StreamOverrideParameters> stream_override,
    bool is_view_source_mode_enabled,
    bool is_same_document_navigation) {
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
      request.AddHTTPOriginIfNeeded(
          WebSecurityOrigin(url::Origin::Create(common_params.referrer.url)));
    }
  }

  if (!web_referrer.IsEmpty() ||
      common_params.referrer.policy != blink::kWebReferrerPolicyDefault) {
    request.SetHTTPReferrer(web_referrer, common_params.referrer.policy);
  }

  request.SetIsSameDocumentNavigation(is_same_document_navigation);
  request.SetPreviewsState(
      static_cast<WebURLRequest::PreviewsState>(common_params.previews_state));

  RequestExtraData* extra_data = new RequestExtraData();
  extra_data->set_stream_override(std::move(stream_override));
  extra_data->set_navigation_initiated_by_renderer(
      request_params.nav_entry_id == 0);
  request.SetExtraData(extra_data);

  base::TimeDelta ui_timestamp = common_params.ui_timestamp - base::TimeTicks();
  request.SetUiStartTime(ui_timestamp.InSecondsF());
  request.SetInputPerfMetricReportPolicy(
      static_cast<WebURLRequest::InputToLoadPerfMetricReportPolicy>(
          common_params.report_type));
  return request;
}
