int UDPSocketWin::RecvFrom(IOBuffer* buf,
                           int buf_len,
                           IPEndPoint* address,
                           const CompletionCallback& callback) {
  DCHECK(CalledOnValidThread());
  DCHECK_NE(INVALID_SOCKET, socket_);
  DCHECK(read_callback_.is_null());
  DCHECK(!recv_from_address_);
  DCHECK(!callback.is_null());   
  DCHECK_GT(buf_len, 0);

  int nread = InternalRecvFrom(buf, buf_len, address);
  if (nread != ERR_IO_PENDING)
    return nread;

  read_callback_ = callback;
  recv_from_address_ = address;
  return ERR_IO_PENDING;
}
