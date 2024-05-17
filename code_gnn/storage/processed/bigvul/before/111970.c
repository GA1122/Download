  bool ShouldRunMigrationTest() const {
    if (!ServerSupportsNotificationControl() ||
        !ServerSupportsErrorTriggering()) {
      LOG(WARNING) << "Test skipped in this server environment.";
      return false;
    }
    return true;
  }
