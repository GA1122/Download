void WebContentsImpl::RendererResponsive(
    RenderWidgetHostImpl* render_widget_host) {
  RenderProcessHost* hung_process = render_widget_host->GetProcess();

  if (delegate_)
    delegate_->RendererResponsive(this, hung_process);
}
