 void WebSocketJob::RetryPendingIO() {
  int result = TrySpdyStream();

  if (result != ERR_IO_PENDING)
    CompleteIO(result);
}
