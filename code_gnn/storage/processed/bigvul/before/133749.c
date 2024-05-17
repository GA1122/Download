void SSLClientSocketOpenSSL::DoReadCallback(int rv) {
  user_read_buf_ = NULL;
  user_read_buf_len_ = 0;
  base::ResetAndReturn(&user_read_callback_).Run(rv);
}
