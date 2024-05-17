HostPortAllocatorSession::HostPortAllocatorSession(
    cricket::HttpPortAllocatorBase* allocator,
    int component,
    const std::string& ice_username_fragment,
    const std::string& ice_password,
    const std::vector<talk_base::SocketAddress>& stun_hosts,
    const std::vector<std::string>& relay_hosts,
    const std::string& relay,
    const scoped_refptr<net::URLRequestContextGetter>& url_context)
    : HttpPortAllocatorSessionBase(
        allocator, component, ice_username_fragment, ice_password,
        stun_hosts, relay_hosts, relay, ""),
      url_context_(url_context) {
}
