NavigationRequest::CheckContentSecurityPolicyFrameSrc(bool is_redirect) {
  if (common_params_.url.SchemeIs(url::kAboutScheme))
    return CONTENT_SECURITY_POLICY_CHECK_PASSED;

  if (common_params_.should_check_main_world_csp ==
      CSPDisposition::DO_NOT_CHECK) {
    return CONTENT_SECURITY_POLICY_CHECK_PASSED;
  }

  if (frame_tree_node()->IsMainFrame())
    return CONTENT_SECURITY_POLICY_CHECK_PASSED;

  FrameTreeNode* parent_ftn = frame_tree_node()->parent();
  DCHECK(parent_ftn);
  RenderFrameHostImpl* parent = parent_ftn->current_frame_host();
  DCHECK(parent);


  bool allowed = parent->IsAllowedByCsp(
      CSPDirective::FrameSrc, common_params_.url, is_redirect,
      common_params_.source_location.value_or(SourceLocation()),
      CSPContext::CHECK_REPORT_ONLY_CSP);

  DCHECK(allowed);

  if (!is_redirect) {
    GURL new_url;
    if (parent->ShouldModifyRequestUrlForCsp(
            common_params_.url, true  , &new_url)) {
      common_params_.url = new_url;
      request_params_.original_url = new_url;
    }
  }

  if (parent->IsAllowedByCsp(
          CSPDirective::FrameSrc, common_params_.url, is_redirect,
          common_params_.source_location.value_or(SourceLocation()),
          CSPContext::CHECK_ENFORCED_CSP)) {
    return CONTENT_SECURITY_POLICY_CHECK_PASSED;
  }

  return CONTENT_SECURITY_POLICY_CHECK_FAILED;
}
