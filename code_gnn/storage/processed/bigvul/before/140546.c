int SpdyProxyClientSocket::DoSendRequestComplete(int result) {
  if (result < 0)
    return result;

  next_state_ = STATE_READ_REPLY_COMPLETE;
  return ERR_IO_PENDING;
}
