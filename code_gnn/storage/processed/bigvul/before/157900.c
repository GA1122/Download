void RenderViewImpl::AttachWebFrameWidget(blink::WebFrameWidget* frame_widget) {
  DCHECK(!frame_widget_);
  frame_widget_ = frame_widget;
}
