void TestTransactionConsumer::Start(const HttpRequestInfo* request,
                                    const BoundNetLog& net_log) {
  state_ = STARTING;
  int result = trans_->Start(
      request, base::Bind(&TestTransactionConsumer::OnIOComplete,
                          base::Unretained(this)), net_log);
  if (result != ERR_IO_PENDING)
    DidStart(result);
}
