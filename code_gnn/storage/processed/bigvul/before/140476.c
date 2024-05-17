scoped_refptr<HttpNetworkSession> SetupSessionForGroupNameTests(
    NextProto next_proto,
    SpdySessionDependencies* session_deps_) {
  scoped_refptr<HttpNetworkSession> session(CreateSession(session_deps_));

  base::WeakPtr<HttpServerProperties> http_server_properties =
      session->http_server_properties();
  http_server_properties->SetAlternateProtocol(
      HostPortPair("host.with.alternate", 80), 443,
      AlternateProtocolFromNextProto(next_proto), 1);

  return session;
}
