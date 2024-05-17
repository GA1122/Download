int SpdyProxyClientSocket::GetLocalAddress(IPEndPoint* address) const {
  if (!IsConnected())
    return ERR_SOCKET_NOT_CONNECTED;
  return spdy_stream_->GetLocalAddress(address);
}
