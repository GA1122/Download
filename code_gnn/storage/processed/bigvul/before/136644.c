NavigationPolicy FrameLoader::CheckLoadCanStart(
    FrameLoadRequest& frame_load_request,
    FrameLoadType type,
    NavigationPolicy navigation_policy,
    NavigationType navigation_type) {
  if (frame_->GetDocument()->PageDismissalEventBeingDispatched() !=
      Document::kNoDismissal) {
    return kNavigationPolicyIgnore;
  }

  ResourceRequest& resource_request = frame_load_request.GetResourceRequest();
  RecordLatestRequiredCSP();
  Settings* settings = frame_->GetSettings();
  MaybeCheckCSP(
      resource_request, navigation_type, frame_, navigation_policy,
      frame_load_request.ShouldCheckMainWorldContentSecurityPolicy() ==
          kCheckContentSecurityPolicy,
      settings && settings->GetBrowserSideNavigationEnabled(),
      ContentSecurityPolicy::CheckHeaderType::kCheckReportOnly);
  ModifyRequestForCSP(resource_request, frame_load_request.OriginDocument());

  WebTriggeringEventInfo triggering_event_info =
      WebTriggeringEventInfo::kNotFromEvent;
  if (frame_load_request.TriggeringEvent()) {
    triggering_event_info = frame_load_request.TriggeringEvent()->isTrusted()
                                ? WebTriggeringEventInfo::kFromTrustedEvent
                                : WebTriggeringEventInfo::kFromUntrustedEvent;
  }
  return ShouldContinueForNavigationPolicy(
      resource_request, frame_load_request.OriginDocument(),
      frame_load_request.GetSubstituteData(), nullptr,
      frame_load_request.ShouldCheckMainWorldContentSecurityPolicy(),
      navigation_type, navigation_policy, type,
      frame_load_request.ClientRedirect() ==
          ClientRedirectPolicy::kClientRedirect,
      triggering_event_info, frame_load_request.Form());
}
