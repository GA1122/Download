int Channel::ChannelImpl::GetClientFileDescriptor() const {
  return client_pipe_;
}
