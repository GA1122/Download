void WebContentsImpl::RendererUnresponsive(
    RenderWidgetHostImpl* render_widget_host) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  RenderFrameHostImpl* rfhi =
      static_cast<RenderFrameHostImpl*>(GetRenderViewHost()->GetMainFrame());

  if (DevToolsAgentHost::IsDebuggerAttached(this))
    return;

  if (rfhi->is_waiting_for_beforeunload_ack() ||
      rfhi->IsWaitingForUnloadACK()) {
    GetRenderViewHost()->set_sudden_termination_allowed(true);

    if (!GetRenderManager()->ShouldCloseTabOnUnresponsiveRenderer())
      return;

    bool close = true;
    if (rfhi->is_waiting_for_beforeunload_ack() && delegate_) {
      delegate_->BeforeUnloadFired(this, true, &close);
    }
    if (close)
      Close();
    return;
  }

  if (!GetRenderViewHost() || !GetRenderViewHost()->IsRenderViewLive())
    return;

  if (delegate_)
    delegate_->RendererUnresponsive(this);
}
