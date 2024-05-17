net::Error NavigationRequest::CheckContentSecurityPolicy(
    bool has_followed_redirect,
    bool url_upgraded_after_redirect,
    bool is_response_check) {
  if (common_params_.url.SchemeIs(url::kAboutScheme))
    return net::OK;

  if (common_params_.initiator_csp_info.should_check_main_world_csp ==
      CSPDisposition::DO_NOT_CHECK) {
    return net::OK;
  }

  FrameTreeNode* parent_ftn = frame_tree_node()->parent();
  RenderFrameHostImpl* parent =
      parent_ftn ? parent_ftn->current_frame_host() : nullptr;

  initiator_csp_context_->SetReportingRenderFrameHost(
      frame_tree_node()->current_frame_host());


  net::Error report_only_csp_status = CheckCSPDirectives(
      parent, has_followed_redirect, url_upgraded_after_redirect,
      is_response_check, CSPContext::CHECK_REPORT_ONLY_CSP);

  if (!has_followed_redirect && !frame_tree_node()->IsMainFrame()) {
    if (parent &&
        parent->ShouldModifyRequestUrlForCsp(true  )) {
      upgrade_if_insecure_ = true;
      parent->ModifyRequestUrlForCsp(&common_params_.url);
      commit_params_.original_url = common_params_.url;
    }
  }

  net::Error enforced_csp_status = CheckCSPDirectives(
      parent, has_followed_redirect, url_upgraded_after_redirect,
      is_response_check, CSPContext::CHECK_ENFORCED_CSP);
  if (enforced_csp_status != net::OK)
    return enforced_csp_status;
  return report_only_csp_status;
}
