std::vector<DataReductionProxyServer> GetProxiesForHTTP(
    const data_reduction_proxy::ProxyConfig& proxy_config) {
  std::vector<DataReductionProxyServer> proxies;
  for (const auto& server : proxy_config.http_proxy_servers()) {
    if (server.scheme() != ProxyServer_ProxyScheme_UNSPECIFIED) {
      proxies.push_back(DataReductionProxyServer(net::ProxyServer(
          protobuf_parser::SchemeFromProxyScheme(server.scheme()),
          net::HostPortPair(server.host(), server.port()),
           
          server.scheme() == ProxyServer_ProxyScheme_HTTPS)));
    }
  }

  return proxies;
}
