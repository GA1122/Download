  ~DBusPreEarlyInit() {
    DBusThreadManager::Shutdown();
    SystemSaltGetter::Shutdown();
  }
