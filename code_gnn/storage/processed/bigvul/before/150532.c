  void SetStatusCodeForURLsFromHostPortPair(
      const net::HostPortPair& host_port_pair,
      net::HttpStatusCode status_code) {
    base::AutoLock auto_lock(lock_);
    special_host_port_pair_ = host_port_pair;
    special_status_code_ = status_code;
  }
