  net::EmbeddedTestServer::Type GetTestServerType() {
    if (std::get<0>(GetParam()) == ProxyServer_ProxyScheme_HTTP)
      return net::EmbeddedTestServer::TYPE_HTTP;
    return net::EmbeddedTestServer::TYPE_HTTPS;
  }
