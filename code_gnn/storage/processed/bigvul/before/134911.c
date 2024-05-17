void ChromeBrowserMainPartsChromeos::PostMainMessageLoopStart() {
  device_event_log::Initialize(0  );

  g_browser_process->platform_part()->InitializeCrosComponentManager();

  dbus_services_.reset(new internal::DBusServices(parameters()));

  memory_kills_monitor_ = memory::MemoryKillsMonitor::Initialize();

  ChromeBrowserMainPartsLinux::PostMainMessageLoopStart();
}
