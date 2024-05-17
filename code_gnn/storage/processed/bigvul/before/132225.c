CommonNavigationParams MakeCommonNavigationParams(
    blink::WebURLRequest* request,
    bool should_replace_current_entry) {
  const RequestExtraData kEmptyData;
  const RequestExtraData* extra_data =
      static_cast<RequestExtraData*>(request->extraData());
  if (!extra_data)
    extra_data = &kEmptyData;
  Referrer referrer(
      GURL(request->httpHeaderField(WebString::fromUTF8("Referer")).latin1()),
      request->referrerPolicy());

  base::TimeTicks ui_timestamp =
      base::TimeTicks() + base::TimeDelta::FromSecondsD(request->uiStartTime());
  FrameMsg_UILoadMetricsReportType::Value report_type =
      static_cast<FrameMsg_UILoadMetricsReportType::Value>(
          request->inputPerfMetricReportPolicy());
  return CommonNavigationParams(
      request->url(), referrer, extra_data->transition_type(),
      FrameMsg_Navigate_Type::NORMAL, true, should_replace_current_entry,
      ui_timestamp, report_type, GURL(), GURL());
}
