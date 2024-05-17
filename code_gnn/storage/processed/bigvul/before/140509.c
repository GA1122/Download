int HttpProxyClientSocket::DoTCPRestartComplete(int result) {
  if (result != OK)
    return result;

  next_state_ = STATE_GENERATE_AUTH_TOKEN;
  return result;
}
