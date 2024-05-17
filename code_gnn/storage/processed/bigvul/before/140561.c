void SpdyProxyClientSocket::OnDataSent()  {
  DCHECK(!write_callback_.is_null());

  int rv = write_buffer_len_;
  write_buffer_len_ = 0;

  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&SpdyProxyClientSocket::RunCallback,
                 write_callback_weak_factory_.GetWeakPtr(),
                 ResetAndReturn(&write_callback_),
                 rv));
}
