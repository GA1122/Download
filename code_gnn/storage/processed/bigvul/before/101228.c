SyncProtocolErrorType ConvertSyncProtocolErrorTypePBToLocalType(
    const sync_pb::ClientToServerResponse::ErrorType& error_type) {
  switch (error_type) {
    case ClientToServerResponse::SUCCESS:
      return browser_sync::SYNC_SUCCESS;
    case ClientToServerResponse::NOT_MY_BIRTHDAY:
      return browser_sync::NOT_MY_BIRTHDAY;
    case ClientToServerResponse::THROTTLED:
      return browser_sync::THROTTLED;
    case ClientToServerResponse::CLEAR_PENDING:
      return browser_sync::CLEAR_PENDING;
    case ClientToServerResponse::TRANSIENT_ERROR:
      return browser_sync::TRANSIENT_ERROR;
    case ClientToServerResponse::MIGRATION_DONE:
      return browser_sync::MIGRATION_DONE;
    case ClientToServerResponse::UNKNOWN:
      return browser_sync::UNKNOWN_ERROR;
    case ClientToServerResponse::USER_NOT_ACTIVATED:
    case ClientToServerResponse::AUTH_INVALID:
    case ClientToServerResponse::ACCESS_DENIED:
      return browser_sync::INVALID_CREDENTIAL;
    default:
      NOTREACHED();
      return browser_sync::UNKNOWN_ERROR;
  }
}
