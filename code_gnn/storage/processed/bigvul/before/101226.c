browser_sync::SyncProtocolError ConvertErrorPBToLocalType(
    const sync_pb::ClientToServerResponse::Error& error) {
  browser_sync::SyncProtocolError sync_protocol_error;
  sync_protocol_error.error_type = ConvertSyncProtocolErrorTypePBToLocalType(
      error.error_type());
  sync_protocol_error.error_description = error.error_description();
  sync_protocol_error.url = error.url();
  sync_protocol_error.action = ConvertClientActionPBToLocalClientAction(
      error.action());
  return sync_protocol_error;
}
