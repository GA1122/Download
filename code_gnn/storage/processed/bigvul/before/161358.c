String getProtocol(const GURL& url, const network::ResourceResponseHead& head) {
  std::string protocol = head.alpn_negotiated_protocol;
  if (protocol.empty() || protocol == "unknown") {
    if (head.was_fetched_via_spdy) {
      protocol = "spdy";
    } else if (url.SchemeIsHTTPOrHTTPS()) {
      protocol = "http";
      if (head.headers->GetHttpVersion() == net::HttpVersion(0, 9))
        protocol = "http/0.9";
      else if (head.headers->GetHttpVersion() == net::HttpVersion(1, 0))
        protocol = "http/1.0";
      else if (head.headers->GetHttpVersion() == net::HttpVersion(1, 1))
        protocol = "http/1.1";
    } else {
      protocol = url.scheme();
    }
  }
  return protocol;
}
