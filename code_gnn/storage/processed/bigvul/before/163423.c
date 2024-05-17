RenderThreadImpl::render_frame_message_filter() {
  if (!render_frame_message_filter_)
    GetChannel()->GetRemoteAssociatedInterface(&render_frame_message_filter_);
  return render_frame_message_filter_.get();
}
