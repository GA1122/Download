void SSLClientSocketOpenSSL::DoWriteCallback(int rv) {
  user_write_buf_ = NULL;
  user_write_buf_len_ = 0;
  base::ResetAndReturn(&user_write_callback_).Run(rv);
}
