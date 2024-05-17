browser_sync::ClientAction ConvertClientActionPBToLocalClientAction(
    const sync_pb::ClientToServerResponse::Error::Action& action) {
  switch (action) {
    case ClientToServerResponse::Error::UPGRADE_CLIENT:
      return browser_sync::UPGRADE_CLIENT;
    case ClientToServerResponse::Error::CLEAR_USER_DATA_AND_RESYNC:
      return browser_sync::CLEAR_USER_DATA_AND_RESYNC;
    case ClientToServerResponse::Error::ENABLE_SYNC_ON_ACCOUNT:
      return browser_sync::ENABLE_SYNC_ON_ACCOUNT;
    case ClientToServerResponse::Error::STOP_AND_RESTART_SYNC:
      return browser_sync::STOP_AND_RESTART_SYNC;
    case ClientToServerResponse::Error::DISABLE_SYNC_ON_CLIENT:
      return browser_sync::DISABLE_SYNC_ON_CLIENT;
    case ClientToServerResponse::Error::UNKNOWN_ACTION:
      return browser_sync::UNKNOWN_ACTION;
    default:
      NOTREACHED();
      return browser_sync::UNKNOWN_ACTION;
  }
}
