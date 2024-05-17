  DBusPreEarlyInit() {
    SystemSaltGetter::Initialize();

    DBusThreadManager::Initialize(DBusThreadManager::kAll);

    DeviceSettingsService::Initialize();
  }
