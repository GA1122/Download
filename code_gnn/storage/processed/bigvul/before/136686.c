NavigationPolicy FrameLoader::ShouldContinueForRedirectNavigationPolicy(
    const ResourceRequest& request,
    const SubstituteData& substitute_data,
    DocumentLoader* loader,
    ContentSecurityPolicyDisposition
        should_check_main_world_content_security_policy,
    NavigationType type,
    NavigationPolicy policy,
    FrameLoadType frame_load_type,
    bool is_client_redirect,
    HTMLFormElement* form) {
  Settings* settings = frame_->GetSettings();
  MaybeCheckCSP(request, type, frame_, policy,
                should_check_main_world_content_security_policy ==
                    kCheckContentSecurityPolicy,
                settings && settings->GetBrowserSideNavigationEnabled(),
                ContentSecurityPolicy::CheckHeaderType::kCheckReportOnly);

  return ShouldContinueForNavigationPolicy(
      request,
      nullptr,   
      substitute_data, loader, should_check_main_world_content_security_policy,
      type, policy, frame_load_type, is_client_redirect,
      WebTriggeringEventInfo::kNotFromEvent, form);
}
