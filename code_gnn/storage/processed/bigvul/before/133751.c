bool SSLClientSocketOpenSSL::DoTransportIO() {
  bool network_moved = false;
  int rv;
  do {
    rv = BufferSend();
    if (rv != ERR_IO_PENDING && rv != 0)
      network_moved = true;
  } while (rv > 0);
  if (!transport_recv_eof_ && BufferRecv() != ERR_IO_PENDING)
    network_moved = true;
  return network_moved;
}
