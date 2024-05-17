scoped_ptr<HostPortAllocator> HostPortAllocator::Create(
    const scoped_refptr<net::URLRequestContextGetter>& url_context,
    const NetworkSettings& network_settings) {
  scoped_ptr<talk_base::NetworkManager> network_manager(
      new talk_base::BasicNetworkManager());
  scoped_ptr<talk_base::PacketSocketFactory> socket_factory(
      new talk_base::BasicPacketSocketFactory());
  scoped_ptr<HostPortAllocator> result(
      new HostPortAllocator(url_context, network_manager.Pass(),
                            socket_factory.Pass()));

  int flags = cricket::PORTALLOCATOR_DISABLE_TCP;
  if (network_settings.nat_traversal_mode !=
      NetworkSettings::NAT_TRAVERSAL_ENABLED) {
    flags |= cricket::PORTALLOCATOR_DISABLE_STUN |
        cricket::PORTALLOCATOR_DISABLE_RELAY;
  }
  result->set_flags(flags);
  result->SetPortRange(network_settings.min_port,
                       network_settings.max_port);

  return result.Pass();
}
