static NavigationPolicy MaybeCheckCSP(
    const ResourceRequest& request,
    NavigationType type,
    LocalFrame* frame,
    NavigationPolicy policy,
    bool should_check_main_world_content_security_policy,
    bool browser_side_navigation_enabled,
    ContentSecurityPolicy::CheckHeaderType check_header_type) {
  if (policy == kNavigationPolicyCurrentTab &&
      should_check_main_world_content_security_policy &&
      !browser_side_navigation_enabled) {
    Frame* parent_frame = frame->Tree().Parent();
    if (parent_frame) {
      ContentSecurityPolicy* parent_policy =
          parent_frame->GetSecurityContext()->GetContentSecurityPolicy();
      if (!parent_policy->AllowFrameFromSource(
              request.Url(), request.GetRedirectStatus(),
              SecurityViolationReportingPolicy::kReport, check_header_type)) {
        frame->GetDocument()->EnforceSandboxFlags(kSandboxOrigin);
        frame->Owner()->DispatchLoad();
        return kNavigationPolicyIgnore;
      }
    }
  }

  bool is_form_submission = type == kNavigationTypeFormSubmitted ||
                            type == kNavigationTypeFormResubmitted;
  if (is_form_submission &&
      !browser_side_navigation_enabled &&
      !frame->GetDocument()->GetContentSecurityPolicy()->AllowFormAction(
          request.Url(), request.GetRedirectStatus(),
          SecurityViolationReportingPolicy::kReport, check_header_type)) {
    return kNavigationPolicyIgnore;
  }

  return policy;
}
