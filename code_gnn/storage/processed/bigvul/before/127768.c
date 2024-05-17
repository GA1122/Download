int MockNetworkTransaction::StartInternal(const HttpRequestInfo* request,
                                          const CompletionCallback& callback,
                                          const BoundNetLog& net_log) {
  const MockTransaction* t = FindMockTransaction(request->url);
  if (!t)
    return ERR_FAILED;

  if (!before_network_start_callback_.is_null()) {
    bool defer = false;
    before_network_start_callback_.Run(&defer);
    if (defer)
      return net::ERR_IO_PENDING;
  }

  test_mode_ = t->test_mode;

  if (OK != t->return_code) {
    if (test_mode_ & TEST_MODE_SYNC_NET_START)
      return t->return_code;
    CallbackLater(callback, t->return_code);
    return ERR_IO_PENDING;
  }

  sent_bytes_ = kTotalSentBytes;
  received_bytes_ = kTotalReceivedBytes;

  std::string resp_status = t->status;
  std::string resp_headers = t->response_headers;
  std::string resp_data = t->data;
  if (t->handler)
    (t->handler)(request, &resp_status, &resp_headers, &resp_data);
  if (t->read_handler)
    read_handler_ = t->read_handler;

  std::string header_data = base::StringPrintf(
      "%s\n%s\n", resp_status.c_str(), resp_headers.c_str());
  std::replace(header_data.begin(), header_data.end(), '\n', '\0');

  response_.request_time = transaction_factory_->Now();
  if (!t->request_time.is_null())
    response_.request_time = t->request_time;

  response_.was_cached = false;
  response_.network_accessed = true;

  response_.response_time = transaction_factory_->Now();
  if (!t->response_time.is_null())
    response_.response_time = t->response_time;

  response_.headers = new HttpResponseHeaders(header_data);
  response_.vary_data.Init(*request, *response_.headers.get());
  response_.ssl_info.cert = t->cert;
  response_.ssl_info.cert_status = t->cert_status;
  response_.ssl_info.connection_status = t->ssl_connection_status;
  data_ = resp_data;
  content_length_ = response_.headers->GetContentLength();

  if (net_log.net_log())
    socket_log_id_ = net_log.net_log()->NextID();

  if (request_->load_flags & LOAD_PREFETCH)
    response_.unused_since_prefetch = true;

  if (test_mode_ & TEST_MODE_SYNC_NET_START)
    return OK;

  CallbackLater(callback, OK);
  return ERR_IO_PENDING;
}
