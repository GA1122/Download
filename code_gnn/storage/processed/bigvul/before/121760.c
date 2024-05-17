void UDPSocketLibevent::LogRead(int result,
                                const char* bytes,
                                socklen_t addr_len,
                                const sockaddr* addr) const {
  if (result < 0) {
    net_log_.AddEventWithNetErrorCode(NetLog::TYPE_UDP_RECEIVE_ERROR, result);
    return;
  }

  if (net_log_.IsLoggingAllEvents()) {
    DCHECK(addr_len > 0);
    DCHECK(addr);

    IPEndPoint address;
    bool is_address_valid = address.FromSockAddr(addr, addr_len);
    net_log_.AddEvent(
        NetLog::TYPE_UDP_BYTES_RECEIVED,
        CreateNetLogUDPDataTranferCallback(
            result, bytes,
            is_address_valid ? &address : NULL));
  }

  base::StatsCounter read_bytes("udp.read_bytes");
  read_bytes.Add(result);
}
