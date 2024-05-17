sync_pb::SyncEnums::Action GetClientToServerResponseAction(
    const browser_sync::ClientAction& action) {
  switch (action) {
    case browser_sync::UPGRADE_CLIENT:
      return sync_pb::SyncEnums::UPGRADE_CLIENT;
    case browser_sync::CLEAR_USER_DATA_AND_RESYNC:
      return sync_pb::SyncEnums::CLEAR_USER_DATA_AND_RESYNC;
    case browser_sync::ENABLE_SYNC_ON_ACCOUNT:
      return sync_pb::SyncEnums::ENABLE_SYNC_ON_ACCOUNT;
    case browser_sync::STOP_AND_RESTART_SYNC:
      return sync_pb::SyncEnums::STOP_AND_RESTART_SYNC;
    case browser_sync::DISABLE_SYNC_ON_CLIENT:
      return sync_pb::SyncEnums::DISABLE_SYNC_ON_CLIENT;
    case browser_sync::UNKNOWN_ACTION:
      return sync_pb::SyncEnums::UNKNOWN_ACTION;
    default:
      NOTREACHED();
      return sync_pb::SyncEnums::UNKNOWN_ACTION;
  }
}
