CommandBufferProxyImpl::CreateVideoDecoder(
    media::VideoCodecProfile profile,
    media::VideoDecodeAccelerator::Client* client) {
  int decoder_route_id;
  if (!Send(new GpuCommandBufferMsg_CreateVideoDecoder(route_id_, profile,
                                                       &decoder_route_id))) {
    LOG(ERROR) << "Send(GpuCommandBufferMsg_CreateVideoDecoder) failed";
    return NULL;
  }

  scoped_refptr<GpuVideoDecodeAcceleratorHost> decoder_host =
      new GpuVideoDecodeAcceleratorHost(channel_, decoder_route_id, client);
  bool inserted = video_decoder_hosts_.insert(std::make_pair(
      decoder_route_id, decoder_host)).second;
  DCHECK(inserted);

  channel_->AddRoute(decoder_route_id, decoder_host->AsWeakPtr());

  return decoder_host;
}
