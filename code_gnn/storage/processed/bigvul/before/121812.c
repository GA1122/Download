UDPSocketWin::UDPSocketWin(DatagramSocket::BindType bind_type,
                           const RandIntCallback& rand_int_cb,
                           net::NetLog* net_log,
                           const net::NetLog::Source& source)
    : socket_(INVALID_SOCKET),
      addr_family_(0),
      socket_options_(SOCKET_OPTION_MULTICAST_LOOP),
      multicast_interface_(0),
      multicast_time_to_live_(1),
      bind_type_(bind_type),
      rand_int_cb_(rand_int_cb),
      recv_from_address_(NULL),
      net_log_(BoundNetLog::Make(net_log, NetLog::SOURCE_UDP_SOCKET)) {
  EnsureWinsockInit();
  net_log_.BeginEvent(NetLog::TYPE_SOCKET_ALIVE,
                      source.ToEventParametersCallback());
  if (bind_type == DatagramSocket::RANDOM_BIND)
    DCHECK(!rand_int_cb.is_null());
}