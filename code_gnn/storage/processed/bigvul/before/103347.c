void P2PSocketDispatcherHost::SendNetworkList(
    int routing_id, const net::NetworkInterfaceList& list) {
  Send(new P2PMsg_NetworkList(routing_id, list));
}
