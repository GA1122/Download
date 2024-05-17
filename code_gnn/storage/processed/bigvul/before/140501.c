int HttpProxyClientSocket::DoDrainBodyComplete(int result) {
  if (result < 0)
    return result;

  if (http_stream_parser_->IsResponseBodyComplete())
    return DidDrainBodyForAuthRestart(true);

  next_state_ = STATE_DRAIN_BODY;
  return OK;
}
