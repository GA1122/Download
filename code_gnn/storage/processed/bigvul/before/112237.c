cricket::PortAllocatorSession* HostPortAllocator::CreateSessionInternal(
    int component,
    const std::string& ice_username_fragment,
    const std::string& ice_password) {
  return new HostPortAllocatorSession(
      this, component, ice_username_fragment, ice_password,
      stun_hosts(), relay_hosts(), relay_token(), url_context_);
}
