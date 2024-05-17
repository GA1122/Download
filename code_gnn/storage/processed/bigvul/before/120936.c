int SocketStream::DoResolveHost() {
  next_state_ = STATE_RESOLVE_HOST_COMPLETE;

  DCHECK(!proxy_info_.is_empty());
  if (proxy_info_.is_direct())
    proxy_mode_ = kDirectConnection;
  else if (proxy_info_.proxy_server().is_socks())
    proxy_mode_ = kSOCKSProxy;
  else
    proxy_mode_ = kTunnelProxy;

  HostPortPair host_port_pair;
  if (proxy_mode_ != kDirectConnection) {
    host_port_pair = proxy_info_.proxy_server().host_port_pair();
  } else {
    host_port_pair = HostPortPair::FromURL(url_);
  }

  HostResolver::RequestInfo resolve_info(host_port_pair);

  DCHECK(context_->host_resolver());
  resolver_.reset(new SingleRequestHostResolver(context_->host_resolver()));
  return resolver_->Resolve(
      resolve_info, &addresses_, base::Bind(&SocketStream::OnIOCompleted, this),
      net_log_);
}
