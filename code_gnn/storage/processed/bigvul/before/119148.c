bool CancelableSyncSocket::Shutdown() {
  shutdown_event_.Signal();
  return true;
}
