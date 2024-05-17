bool UDPSocketWin::ReceiveAddressToIPEndpoint(IPEndPoint* address) const {
  SockaddrStorage& storage = core_->recv_addr_storage_;
  return address->FromSockAddr(storage.addr, storage.addr_len);
}
