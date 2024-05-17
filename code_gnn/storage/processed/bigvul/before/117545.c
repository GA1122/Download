ChromeBrowserMainPartsChromeos::~ChromeBrowserMainPartsChromeos() {
  chromeos::disks::DiskMountManager::Shutdown();

  chromeos::DBusThreadManager::Shutdown();

  if (!parameters().ui_task && chromeos::CrosLibrary::Get())
    chromeos::CrosLibrary::Shutdown();

  chromeos::BootTimesLoader::Get()->AddLogoutTimeMarker("LogoutDone",
                                                        false);
  chromeos::BootTimesLoader::Get()->WriteLogoutTimes();
}
