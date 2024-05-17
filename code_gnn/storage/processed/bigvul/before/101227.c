browser_sync::SyncProtocolError ConvertLegacyErrorCodeToNewError(
    const sync_pb::ClientToServerResponse::ErrorType& error_type) {
  browser_sync::SyncProtocolError error;
  error.error_type = ConvertSyncProtocolErrorTypePBToLocalType(error_type);
  if (error_type == ClientToServerResponse::CLEAR_PENDING ||
      error_type == ClientToServerResponse::NOT_MY_BIRTHDAY) {
      error.action = browser_sync::DISABLE_SYNC_ON_CLIENT;
  }   
  return error;
}
