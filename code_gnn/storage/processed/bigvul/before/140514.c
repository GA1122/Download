int HttpProxyClientSocket::GetPeerAddress(IPEndPoint* address) const {
  return transport_->socket()->GetPeerAddress(address);
}
