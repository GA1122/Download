void RenderThreadImpl::WidgetDestroyed() {
  DCHECK_GT(widget_count_, 0);
  DCHECK_GT(widget_count_, hidden_widget_count_);
  widget_count_--;
  if (RendererIsHidden())
    OnRendererHidden();
}
