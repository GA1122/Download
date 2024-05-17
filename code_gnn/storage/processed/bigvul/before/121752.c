void UDPSocketLibevent::DoWriteCallback(int rv) {
  DCHECK_NE(rv, ERR_IO_PENDING);
  DCHECK(!write_callback_.is_null());

  CompletionCallback c = write_callback_;
  write_callback_.Reset();
  c.Run(rv);
}
