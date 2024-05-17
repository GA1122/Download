CommonNavigationParams MakeCommonNavigationParams(
    const blink::WebFrameClient::NavigationPolicyInfo& info,
    int load_flags) {
  Referrer referrer(
      GURL(info.url_request.HttpHeaderField(WebString::FromUTF8("Referer"))
               .Latin1()),
      info.url_request.GetReferrerPolicy());

  base::TimeTicks ui_timestamp =
      base::TimeTicks() +
      base::TimeDelta::FromSecondsD(info.url_request.UiStartTime());
  FrameMsg_UILoadMetricsReportType::Value report_type =
      static_cast<FrameMsg_UILoadMetricsReportType::Value>(
          info.url_request.InputPerfMetricReportPolicy());

  DCHECK(info.navigation_type != blink::kWebNavigationTypeBackForward);

  FrameMsg_Navigate_Type::Value navigation_type =
      FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT;
  if (info.navigation_type == blink::kWebNavigationTypeReload) {
    if (load_flags & net::LOAD_BYPASS_CACHE)
      navigation_type = FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE;
    else
      navigation_type = FrameMsg_Navigate_Type::RELOAD;
  }

  base::Optional<SourceLocation> source_location;
  if (!info.source_location.url.IsNull()) {
    source_location = SourceLocation(info.source_location.url.Latin1(),
                                     info.source_location.line_number,
                                     info.source_location.column_number);
  }

  CSPDisposition should_check_main_world_csp =
      info.should_check_main_world_content_security_policy ==
              blink::kWebContentSecurityPolicyDispositionCheck
          ? CSPDisposition::CHECK
          : CSPDisposition::DO_NOT_CHECK;

  const RequestExtraData* extra_data =
      static_cast<RequestExtraData*>(info.url_request.GetExtraData());
  DCHECK(extra_data);
  return CommonNavigationParams(
      info.url_request.Url(), referrer, extra_data->transition_type(),
      navigation_type, true, info.replaces_current_history_item, ui_timestamp,
      report_type, GURL(), GURL(),
      static_cast<PreviewsState>(info.url_request.GetPreviewsState()),
      base::TimeTicks::Now(), info.url_request.HttpMethod().Latin1(),
      GetRequestBodyForWebURLRequest(info.url_request), source_location,
      should_check_main_world_csp, false  ,
      info.url_request.HasUserGesture(),
      info.url_request.GetSuggestedFilename().has_value()
          ? base::Optional<std::string>(
                info.url_request.GetSuggestedFilename()->Utf8())
          : base::nullopt);
}
