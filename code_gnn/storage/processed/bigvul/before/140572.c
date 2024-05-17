SpdyProxyClientSocket::SpdyProxyClientSocket(
    const base::WeakPtr<SpdyStream>& spdy_stream,
    const std::string& user_agent,
    const HostPortPair& endpoint,
    const GURL& url,
    const HostPortPair& proxy_server,
    const BoundNetLog& source_net_log,
    HttpAuthCache* auth_cache,
    HttpAuthHandlerFactory* auth_handler_factory)
    : next_state_(STATE_DISCONNECTED),
      spdy_stream_(spdy_stream),
      endpoint_(endpoint),
      auth_(new HttpAuthController(HttpAuth::AUTH_PROXY,
                                   GURL("https://" + proxy_server.ToString()),
                                   auth_cache,
                                   auth_handler_factory)),
      user_buffer_len_(0),
      write_buffer_len_(0),
      was_ever_used_(false),
      redirect_has_load_timing_info_(false),
      net_log_(BoundNetLog::Make(spdy_stream->net_log().net_log(),
                                 NetLog::SOURCE_PROXY_CLIENT_SOCKET)),
      weak_factory_(this),
      write_callback_weak_factory_(this) {
  request_.method = "CONNECT";
  request_.url = url;
  if (!user_agent.empty())
    request_.extra_headers.SetHeader(HttpRequestHeaders::kUserAgent,
                                     user_agent);

  net_log_.BeginEvent(NetLog::TYPE_SOCKET_ALIVE,
                      source_net_log.source().ToEventParametersCallback());
  net_log_.AddEvent(
      NetLog::TYPE_SPDY_PROXY_CLIENT_SESSION,
      spdy_stream->net_log().source().ToEventParametersCallback());

  spdy_stream_->SetDelegate(this);
  was_ever_used_ = spdy_stream_->WasEverUsed();
}
