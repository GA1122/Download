void WebSocketJob::CompleteIO(int result) {
  if (callback_) {
    net::CompletionCallback* callback = callback_;
    callback_ = NULL;
    callback->Run(result);
    Release();   
  }
}
