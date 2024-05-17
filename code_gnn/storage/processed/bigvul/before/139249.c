void VRDisplay::Dispose() {
  display_client_binding_.Close();
  vr_v_sync_provider_.reset();
}
