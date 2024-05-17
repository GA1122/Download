blink::WebWidget* RenderViewImpl::GetWebWidget() const {
  if (frame_widget_)
    return frame_widget_;

  return RenderWidget::GetWebWidget();
}
