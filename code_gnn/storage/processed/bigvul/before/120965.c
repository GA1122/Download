GURL SocketStream::ProxyAuthOrigin() const {
  DCHECK(!proxy_info_.is_empty());
  return GURL("http://" +
              proxy_info_.proxy_server().host_port_pair().ToString());
}
