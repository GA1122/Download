void GpuCommandBufferStub::OnDestroyVideoDecoder(int decoder_route_id) {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnDestroyVideoDecoder");
  channel_->RemoveRoute(decoder_route_id);
  video_decoders_.Remove(decoder_route_id);
}
