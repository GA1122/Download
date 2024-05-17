int UDPSocketLibevent::SetSocketOptions() {
  int true_value = 1;
  if (socket_options_ & SOCKET_OPTION_REUSE_ADDRESS) {
    int rv = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &true_value,
                        sizeof(true_value));
    if (rv < 0)
      return MapSystemError(errno);
  }
  if (socket_options_ & SOCKET_OPTION_BROADCAST) {
    int rv;
#if defined(OS_MACOSX)
    rv = setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, &true_value,
                    sizeof(true_value));
#else
    rv = setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, &true_value,
                    sizeof(true_value));
#endif   
    if (rv < 0)
      return MapSystemError(errno);
  }

  if (!(socket_options_ & SOCKET_OPTION_MULTICAST_LOOP)) {
    int rv;
    if (addr_family_ == AF_INET) {
      u_char loop = 0;
      rv = setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_LOOP,
                      &loop, sizeof(loop));
    } else {
      u_int loop = 0;
      rv = setsockopt(socket_, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                      &loop, sizeof(loop));
    }
    if (rv < 0)
      return MapSystemError(errno);
  }
  if (multicast_time_to_live_ != IP_DEFAULT_MULTICAST_TTL) {
    int rv;
    if (addr_family_ == AF_INET) {
      u_char ttl = multicast_time_to_live_;
      rv = setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_TTL,
                      &ttl, sizeof(ttl));
    } else {
      int ttl = multicast_time_to_live_;
      rv = setsockopt(socket_, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
                      &ttl, sizeof(ttl));
    }
    if (rv < 0)
      return MapSystemError(errno);
  }
  if (multicast_interface_ != 0) {
    switch (addr_family_) {
      case AF_INET: {
#if !defined(OS_MACOSX)
        ip_mreqn mreq;
        mreq.imr_ifindex = multicast_interface_;
        mreq.imr_address.s_addr = htonl(INADDR_ANY);
#else
        ip_mreq mreq;
        int error = GetIPv4AddressFromIndex(socket_, multicast_interface_,
                                            &mreq.imr_interface.s_addr);
        if (error != OK)
          return error;
#endif
        int rv = setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_IF,
                            reinterpret_cast<const char*>(&mreq), sizeof(mreq));
        if (rv)
          return MapSystemError(errno);
        break;
      }
      case AF_INET6: {
        uint32 interface_index = multicast_interface_;
        int rv = setsockopt(socket_, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                            reinterpret_cast<const char*>(&interface_index),
                            sizeof(interface_index));
        if (rv)
          return MapSystemError(errno);
        break;
      }
      default:
        NOTREACHED() << "Invalid address family";
        return ERR_ADDRESS_INVALID;
    }
  }
  return OK;
}
