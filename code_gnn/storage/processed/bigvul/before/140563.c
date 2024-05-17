void SpdyProxyClientSocket::OnRequestHeadersSent() {
  DCHECK_EQ(next_state_, STATE_SEND_REQUEST_COMPLETE);

  OnIOComplete(OK);
}
