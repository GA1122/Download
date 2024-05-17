void RequestSender::OnResponseStarted(int response_code,
                                      int64_t content_length) {
  response_code_ = response_code;
}
