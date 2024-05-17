 void ProxyClientSocket::BuildTunnelRequest(
     const HttpRequestInfo& request_info,
    const HttpRequestHeaders& auth_headers,
    const HostPortPair& endpoint,
    std::string* request_line,
    HttpRequestHeaders* request_headers) {
  *request_line = base::StringPrintf(
      "CONNECT %s HTTP/1.1\r\n", endpoint.ToString().c_str());
  request_headers->SetHeader(HttpRequestHeaders::kHost,
                             GetHostAndOptionalPort(request_info.url));
  request_headers->SetHeader(HttpRequestHeaders::kProxyConnection,
                             "keep-alive");

  std::string user_agent;
  if (request_info.extra_headers.GetHeader(HttpRequestHeaders::kUserAgent,
                                            &user_agent))
    request_headers->SetHeader(HttpRequestHeaders::kUserAgent, user_agent);

  request_headers->MergeFrom(auth_headers);
}
