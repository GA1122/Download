NavigationPolicy FrameLoader::ShouldContinueForRedirectNavigationPolicy(
    const ResourceRequest& request,
    const SubstituteData& substitute_data,
    DocumentLoader* loader,
    ContentSecurityPolicyDisposition
        should_check_main_world_content_security_policy,
    WebNavigationType type,
    NavigationPolicy policy,
    WebFrameLoadType frame_load_type,
    bool is_client_redirect,
    HTMLFormElement* form) {
  return ShouldContinueForNavigationPolicy(
      request,
      nullptr,   
      substitute_data, loader, should_check_main_world_content_security_policy,
      type, policy, frame_load_type, is_client_redirect,
      WebTriggeringEventInfo::kNotFromEvent, form, nullptr  ,
      true  );
}
