net::Error NavigationRequest::CheckCSPDirectives(
    RenderFrameHostImpl* parent,
    bool is_redirect,
    bool url_upgraded_after_redirect,
    bool is_response_check,
    CSPContext::CheckCSPDisposition disposition) {
  bool navigate_to_allowed = IsAllowedByCSPDirective(
      initiator_csp_context_.get(), CSPDirective::NavigateTo, is_redirect,
      url_upgraded_after_redirect, is_response_check, disposition);

  bool frame_src_allowed = true;
  if (parent) {
    frame_src_allowed = IsAllowedByCSPDirective(
        parent, CSPDirective::FrameSrc, is_redirect,
        url_upgraded_after_redirect, is_response_check, disposition);
  }

  if (navigate_to_allowed && frame_src_allowed)
    return net::OK;

  if (!frame_src_allowed)
    return net::ERR_BLOCKED_BY_CLIENT;

  return net::ERR_ABORTED;
}
