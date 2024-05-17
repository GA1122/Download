RenderWidgetHostView* WebContentsImpl::GetFullscreenRenderWidgetHostView()
    const {
  if (auto* widget_host = GetFullscreenRenderWidgetHost())
    return widget_host->GetView();
  return nullptr;
}
