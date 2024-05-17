void NetworkHandler::NavigationPreloadResponseReceived(
    const std::string& request_id,
    const GURL& url,
    const network::ResourceResponseHead& head) {
  if (!enabled_)
    return;
  std::unique_ptr<DictionaryValue> headers_dict(DictionaryValue::create());
  size_t iterator = 0;
  std::string name;
  std::string value;
  while (head.headers->EnumerateHeaderLines(&iterator, &name, &value))
    headers_dict->setString(name, value);
  std::unique_ptr<Network::Response> response(
      Network::Response::Create()
          .SetUrl(url.spec())
          .SetStatus(head.headers->response_code())
          .SetStatusText(head.headers->GetStatusText())
          .SetHeaders(Object::fromValue(headers_dict.get(), nullptr))
          .SetMimeType(head.mime_type)
          .SetConnectionReused(head.load_timing.socket_reused)
          .SetConnectionId(head.load_timing.socket_log_id)
          .SetSecurityState(securityState(url, head.cert_status))
          .SetEncodedDataLength(head.encoded_data_length)
          .SetTiming(getTiming(head.load_timing))
          .SetFromDiskCache(!head.load_timing.request_start_time.is_null() &&
                            head.response_time <
                                head.load_timing.request_start_time)
          .Build());
  if (head.raw_request_response_info) {
    if (head.raw_request_response_info->http_status_code) {
      response->SetStatus(head.raw_request_response_info->http_status_code);
      response->SetStatusText(head.raw_request_response_info->http_status_text);
    }
    if (head.raw_request_response_info->request_headers.size()) {
      response->SetRequestHeaders(
          getHeaders(head.raw_request_response_info->request_headers));
    }
    if (!head.raw_request_response_info->request_headers_text.empty()) {
      response->SetRequestHeadersText(
          head.raw_request_response_info->request_headers_text);
    }
    if (head.raw_request_response_info->response_headers.size())
      response->SetHeaders(
          getHeaders(head.raw_request_response_info->response_headers));
    if (!head.raw_request_response_info->response_headers_text.empty())
      response->SetHeadersText(
          head.raw_request_response_info->response_headers_text);
  }
  response->SetProtocol(getProtocol(url, head));
  response->SetRemoteIPAddress(head.socket_address.HostForURL());
  response->SetRemotePort(head.socket_address.port());
  frontend_->ResponseReceived(
      request_id, ""  ,
      base::TimeTicks::Now().ToInternalValue() /
          static_cast<double>(base::Time::kMicrosecondsPerSecond),
      Page::ResourceTypeEnum::Other, std::move(response));
}
