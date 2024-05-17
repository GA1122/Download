std::unique_ptr<HttpNetworkSession> CreateSession(
    SpdySessionDependencies* session_deps) {
  return SpdySessionDependencies::SpdyCreateSession(session_deps);
}
