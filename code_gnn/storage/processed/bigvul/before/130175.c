void RenderFrameHostImpl::Navigate(const FrameMsg_Navigate_Params& params) {
  TRACE_EVENT0("frame_host", "RenderFrameHostImpl::Navigate");
  if (!GetProcess()->IsGuest()) {
    ChildProcessSecurityPolicyImpl::GetInstance()->GrantRequestURL(
        GetProcess()->GetID(), params.url);
    if (params.url.SchemeIs(kDataScheme) &&
        params.base_url_for_data_url.SchemeIs(kFileScheme)) {
      ChildProcessSecurityPolicyImpl::GetInstance()->GrantRequestURL(
          GetProcess()->GetID(), params.base_url_for_data_url);
    }
  }

  if (render_view_host_->navigations_suspended_) {
    DCHECK(!render_view_host_->suspended_nav_params_.get());
    render_view_host_->suspended_nav_params_.reset(
        new FrameMsg_Navigate_Params(params));
  } else {
    render_view_host_->SetState(RenderViewHostImpl::STATE_DEFAULT);

    Send(new FrameMsg_Navigate(routing_id_, params));
  }

  if (!params.url.SchemeIs(kJavaScriptScheme))
    delegate_->DidStartLoading(this);
}
