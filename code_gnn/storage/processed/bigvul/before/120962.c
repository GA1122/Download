void SocketStream::OnReadCompleted(int result) {
  if (result == 0) {
    server_closed_ = true;
  } else if (result > 0 && read_buf_.get()) {
    result = DidReceiveData(result);
  }
  DoLoop(result);
}
