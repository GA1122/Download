void UDPSocketWin::LogRead(int result, const char* bytes) const {
  if (result < 0) {
    net_log_.AddEventWithNetErrorCode(NetLog::TYPE_UDP_RECEIVE_ERROR, result);
    return;
  }

  if (net_log_.IsLoggingAllEvents()) {
    IPEndPoint address;
    bool is_address_valid = ReceiveAddressToIPEndpoint(&address);
    net_log_.AddEvent(
        NetLog::TYPE_UDP_BYTES_RECEIVED,
        CreateNetLogUDPDataTranferCallback(
            result, bytes,
            is_address_valid ? &address : NULL));
  }

  base::StatsCounter read_bytes("udp.read_bytes");
  read_bytes.Add(result);
}
