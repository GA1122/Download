bool CancelableSyncSocket::Close() {
  const bool result = SyncSocket::Close();
  shutdown_event_.Reset();
  return result;
}
