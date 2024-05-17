void MockNetworkTransaction::RunCallback(const CompletionCallback& callback,
                                         int result) {
  callback.Run(result);
}
