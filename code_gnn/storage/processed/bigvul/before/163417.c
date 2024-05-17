void RenderThreadImpl::WidgetCreated() {
  bool renderer_was_hidden = RendererIsHidden();
  widget_count_++;
  if (renderer_was_hidden)
    OnRendererVisible();
}
