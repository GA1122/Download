WebURLRequest CreateURLRequestForNavigation(
    const CommonNavigationParams& common_params,
    scoped_ptr<StreamOverrideParameters> stream_override,
    bool is_view_source_mode_enabled) {
  WebURLRequest request(common_params.url);
  if (is_view_source_mode_enabled)
    request.setCachePolicy(WebURLRequest::ReturnCacheDataElseLoad);

  if (common_params.referrer.url.is_valid()) {
    WebString web_referrer = WebSecurityPolicy::generateReferrerHeader(
        common_params.referrer.policy,
        common_params.url,
        WebString::fromUTF8(common_params.referrer.url.spec()));
    if (!web_referrer.isEmpty())
      request.setHTTPReferrer(web_referrer, common_params.referrer.policy);
  }

  RequestExtraData* extra_data = new RequestExtraData();
  extra_data->set_stream_override(stream_override.Pass());
  request.setExtraData(extra_data);

  base::TimeDelta ui_timestamp = common_params.ui_timestamp - base::TimeTicks();
  request.setUiStartTime(ui_timestamp.InSecondsF());
  request.setInputPerfMetricReportPolicy(
      static_cast<WebURLRequest::InputToLoadPerfMetricReportPolicy>(
          common_params.report_type));
  return request;
}
