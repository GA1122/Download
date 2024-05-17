bool BrokerAddTargetPeer(HANDLE peer_process) {
  return g_broker_services->AddTargetPeer(peer_process) == sandbox::SBOX_ALL_OK;
}
