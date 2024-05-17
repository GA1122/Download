void UDPSocketLibevent::Close() {
  DCHECK(CalledOnValidThread());

  if (!is_connected())
    return;

  read_buf_ = NULL;
  read_buf_len_ = 0;
  read_callback_.Reset();
  recv_from_address_ = NULL;
  write_buf_ = NULL;
  write_buf_len_ = 0;
  write_callback_.Reset();
  send_to_address_.reset();

  bool ok = read_socket_watcher_.StopWatchingFileDescriptor();
  DCHECK(ok);
  ok = write_socket_watcher_.StopWatchingFileDescriptor();
  DCHECK(ok);

  if (IGNORE_EINTR(close(socket_)) < 0)
    PLOG(ERROR) << "close";

  socket_ = kInvalidSocket;
  addr_family_ = 0;
}
