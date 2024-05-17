std::unique_ptr<HttpNetworkSession> SetupSessionForGroupNameTests(
    SpdySessionDependencies* session_deps_) {
  std::unique_ptr<HttpNetworkSession> session(CreateSession(session_deps_));

  HttpServerProperties* http_server_properties =
      session->http_server_properties();
  AlternativeService alternative_service(kProtoHTTP2, "", 444);
  base::Time expiration = base::Time::Now() + base::TimeDelta::FromDays(1);
  http_server_properties->SetHttp2AlternativeService(
      url::SchemeHostPort("https", "host.with.alternate", 443),
      alternative_service, expiration);

  return session;
}
