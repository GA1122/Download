HttpProxyClientSocket::HttpProxyClientSocket(
    ClientSocketHandle* transport_socket,
    const GURL& request_url,
    const std::string& user_agent,
    const HostPortPair& endpoint,
    const HostPortPair& proxy_server,
    HttpAuthCache* http_auth_cache,
    HttpAuthHandlerFactory* http_auth_handler_factory,
    bool tunnel,
    bool using_spdy,
    NextProto protocol_negotiated,
    ProxyDelegate* proxy_delegate,
    bool is_https_proxy)
    : io_callback_(base::Bind(&HttpProxyClientSocket::OnIOComplete,
                              base::Unretained(this))),
      next_state_(STATE_NONE),
      transport_(transport_socket),
      endpoint_(endpoint),
      auth_(tunnel ?
          new HttpAuthController(HttpAuth::AUTH_PROXY,
                                 GURL((is_https_proxy ? "https://" : "http://")
                                      + proxy_server.ToString()),
                                 http_auth_cache,
                                 http_auth_handler_factory)
          : NULL),
      tunnel_(tunnel),
      using_spdy_(using_spdy),
      protocol_negotiated_(protocol_negotiated),
      is_https_proxy_(is_https_proxy),
      redirect_has_load_timing_info_(false),
      proxy_server_(proxy_server),
      proxy_delegate_(proxy_delegate),
      net_log_(transport_socket->socket()->NetLog()) {
  request_.url = request_url;
  request_.method = "GET";
  if (!user_agent.empty())
    request_.extra_headers.SetHeader(HttpRequestHeaders::kUserAgent,
                                     user_agent);
}
