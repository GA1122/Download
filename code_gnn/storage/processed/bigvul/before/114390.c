 void GpuCommandBufferStub::OnDestroyVideoDecoder(int decoder_route_id) {
  channel_->RemoveRoute(decoder_route_id);
  video_decoders_.Remove(decoder_route_id);
}
