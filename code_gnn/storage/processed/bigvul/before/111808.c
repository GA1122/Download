bool SyncBackendHost::IsDownloadingNigoriForTest() const {
  return initialization_state_ == DOWNLOADING_NIGORI;
}
