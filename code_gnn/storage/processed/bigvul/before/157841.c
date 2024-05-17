void WebContentsImpl::RendererUnresponsive(
    RenderWidgetHostImpl* render_widget_host,
    base::RepeatingClosure hang_monitor_restarter) {
  for (auto& observer : observers_)
    observer.OnRendererUnresponsive(render_widget_host->GetProcess());

  if (ShouldIgnoreUnresponsiveRenderer())
    return;

  if (!render_widget_host->renderer_initialized())
    return;

  if (delegate_)
    delegate_->RendererUnresponsive(this, render_widget_host,
                                    std::move(hang_monitor_restarter));
}
