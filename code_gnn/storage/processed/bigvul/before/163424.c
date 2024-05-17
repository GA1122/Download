mojom::RenderMessageFilter* RenderThreadImpl::render_message_filter() {
  if (!render_message_filter_)
    GetChannel()->GetRemoteAssociatedInterface(&render_message_filter_);
  return render_message_filter_.get();
}
