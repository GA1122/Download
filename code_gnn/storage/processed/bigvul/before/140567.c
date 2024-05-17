void SpdyProxyClientSocket::RunCallback(const CompletionCallback& callback,
                                        int result) const {
  callback.Run(result);
}
