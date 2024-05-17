CommonNavigationParams MakeCommonNavigationParams(
    const WebSecurityOrigin& current_origin,
    std::unique_ptr<blink::WebNavigationInfo> info,
    int load_flags,
    bool has_download_sandbox_flag,
    bool from_ad) {
  DCHECK(!info->url_request.RequestorOrigin().IsNull());

  Referrer referrer(
      GURL(info->url_request.HttpHeaderField(WebString::FromUTF8("Referer"))
               .Latin1()),
      info->url_request.GetReferrerPolicy());

  DCHECK(info->navigation_type != blink::kWebNavigationTypeBackForward);

  FrameMsg_Navigate_Type::Value navigation_type =
      FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT;
  if (info->navigation_type == blink::kWebNavigationTypeReload) {
    if (load_flags & net::LOAD_BYPASS_CACHE)
      navigation_type = FrameMsg_Navigate_Type::RELOAD_BYPASSING_CACHE;
    else
      navigation_type = FrameMsg_Navigate_Type::RELOAD;
  }

  base::Optional<SourceLocation> source_location;
  if (!info->source_location.url.IsNull()) {
    source_location = SourceLocation(info->source_location.url.Latin1(),
                                     info->source_location.line_number,
                                     info->source_location.column_number);
  }

  CSPDisposition should_check_main_world_csp =
      info->should_check_main_world_content_security_policy ==
              blink::kWebContentSecurityPolicyDispositionCheck
          ? CSPDisposition::CHECK
          : CSPDisposition::DO_NOT_CHECK;

  const RequestExtraData* extra_data =
      static_cast<RequestExtraData*>(info->url_request.GetExtraData());
  DCHECK(extra_data);

  NavigationDownloadPolicy download_policy;
  RenderFrameImpl::MaybeSetDownloadFramePolicy(
      info->is_opener_navigation, info->url_request, current_origin,
      has_download_sandbox_flag,
      info->blocking_downloads_in_sandbox_without_user_activation_enabled,
      from_ad, &download_policy);

  return CommonNavigationParams(
      info->url_request.Url(), info->url_request.RequestorOrigin(), referrer,
      extra_data->transition_type(), navigation_type, download_policy,
      info->frame_load_type == WebFrameLoadType::kReplaceCurrentItem, GURL(),
      GURL(), static_cast<PreviewsState>(info->url_request.GetPreviewsState()),
      base::TimeTicks::Now(), info->url_request.HttpMethod().Latin1(),
      GetRequestBodyForWebURLRequest(info->url_request), source_location,
      false  , info->url_request.HasUserGesture(),
      InitiatorCSPInfo(should_check_main_world_csp,
                       BuildContentSecurityPolicyList(info->initiator_csp),
                       info->initiator_csp.self_source.has_value()
                           ? base::Optional<CSPSource>(BuildCSPSource(
                                 info->initiator_csp.self_source.value()))
                           : base::nullopt),
      info->href_translate.Latin1(), info->input_start);
}
