void BrowserViewRenderer::ReturnUnusedResource(
    scoped_ptr<ChildFrame> child_frame) {
  if (!child_frame.get() || !child_frame->frame.get())
    return;

  cc::CompositorFrameAck frame_ack;
  cc::TransferableResource::ReturnResources(
      child_frame->frame->delegated_frame_data->resource_list,
      &frame_ack.resources);
  if (compositor_ && !frame_ack.resources.empty())
    compositor_->ReturnResources(frame_ack);
}
