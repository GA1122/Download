void ChromeBrowserMainPartsChromeos::PostDestroyThreads() {
  dbus_services_.reset();

  g_browser_process->platform_part()->ShutdownCrosComponentManager();

  dbus_pre_early_init_.reset();

  system_token_certdb_initializer_.reset();

  ChromeBrowserMainPartsLinux::PostDestroyThreads();

  DeviceSettingsService::Shutdown();
}
