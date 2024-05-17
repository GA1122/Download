WebLocalFrameImpl::~WebLocalFrameImpl() {
  DCHECK(!frame_widget_);
  g_frame_count--;
}
