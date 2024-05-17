ClientConfig CreateConfigForServer(const net::EmbeddedTestServer& server) {
  net::HostPortPair host_port_pair = server.host_port_pair();
  return CreateConfig(kSessionKey, 1000, 0, ProxyServer_ProxyScheme_HTTP,
                      host_port_pair.host(), host_port_pair.port(),
                      ProxyServer_ProxyScheme_HTTP, "fallback.net", 80, 0.5f,
                      false);
}
