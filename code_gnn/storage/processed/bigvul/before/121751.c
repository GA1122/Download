void UDPSocketLibevent::DoReadCallback(int rv) {
  DCHECK_NE(rv, ERR_IO_PENDING);
  DCHECK(!read_callback_.is_null());

  CompletionCallback c = read_callback_;
  read_callback_.Reset();
  c.Run(rv);
}
