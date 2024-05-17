HostPortAllocator::HostPortAllocator(
    const scoped_refptr<net::URLRequestContextGetter>& url_context,
    scoped_ptr<talk_base::NetworkManager> network_manager,
    scoped_ptr<talk_base::PacketSocketFactory> socket_factory)
    : HttpPortAllocatorBase(network_manager.get(), socket_factory.get(), ""),
      url_context_(url_context),
      network_manager_(network_manager.Pass()),
      socket_factory_(socket_factory.Pass()) {
}
