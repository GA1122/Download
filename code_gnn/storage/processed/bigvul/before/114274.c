 int WebGraphicsContext3DCommandBufferImpl::GetChannelID() {
  return host_ ? host_->client_id() : 0;
}
