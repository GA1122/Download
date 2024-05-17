void WebContentsImpl::RendererUnresponsive(
    RenderWidgetHostImpl* render_widget_host) {
  RenderProcessHost* hung_process = render_widget_host->GetProcess();

  for (auto& observer : observers_)
    observer.OnRendererUnresponsive(hung_process);

  if (ShouldIgnoreUnresponsiveRenderer())
    return;

  if (!render_widget_host->renderer_initialized())
    return;

  if (delegate_)
    delegate_->RendererUnresponsive(this, hung_process);
}
