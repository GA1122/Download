void CommandBufferProxyImpl::OnChannelError() {
  for (Decoders::iterator it = video_decoder_hosts_.begin();
       it != video_decoder_hosts_.end(); ++it) {
    it->second->OnChannelError();
  }
  OnDestroyed(gpu::error::kUnknown);
}
