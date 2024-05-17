bool ShouldShowActionOnUI(
    const browser_sync::SyncProtocolError& error) {
  return (error.action != browser_sync::UNKNOWN_ACTION &&
          error.action != browser_sync::DISABLE_SYNC_ON_CLIENT);
}
