int HttpProxyClientSocket::SetSendBufferSize(int32 size) {
  return transport_->socket()->SetSendBufferSize(size);
}
