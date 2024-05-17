void VRDisplay::OnVSyncConnectionError() {
  vr_v_sync_provider_.reset();
  if (v_sync_connection_failed_)
    return;
  ConnectVSyncProvider();
  v_sync_connection_failed_ = true;
}
