    GetClientToServerResponseErrorType(
        browser_sync::SyncProtocolErrorType error) {
  switch (error) {
    case browser_sync::SYNC_SUCCESS:
      return sync_pb::SyncEnums::SUCCESS;
    case browser_sync::NOT_MY_BIRTHDAY:
      return sync_pb::SyncEnums::NOT_MY_BIRTHDAY;
    case browser_sync::THROTTLED:
      return sync_pb::SyncEnums::THROTTLED;
    case browser_sync::CLEAR_PENDING:
      return sync_pb::SyncEnums::CLEAR_PENDING;
    case browser_sync::TRANSIENT_ERROR:
      return sync_pb::SyncEnums::TRANSIENT_ERROR;
    case browser_sync::MIGRATION_DONE:
      return sync_pb::SyncEnums::MIGRATION_DONE;
    case browser_sync::UNKNOWN_ERROR:
      return sync_pb::SyncEnums::UNKNOWN;
    default:
      NOTREACHED();
      return sync_pb::SyncEnums::UNKNOWN;
  }
}
