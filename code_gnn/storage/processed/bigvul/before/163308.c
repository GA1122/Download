RenderThreadImpl::CreateExternalBeginFrameSource(int routing_id) {
  return std::make_unique<CompositorExternalBeginFrameSource>(
      compositor_message_filter_.get(), sync_message_filter(), routing_id);
}
