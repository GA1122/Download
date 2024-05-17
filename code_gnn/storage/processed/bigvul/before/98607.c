bool BackingStoreWin::ColorManagementEnabled() {
  static bool enabled = false;
  static bool checked = false;
  if (!checked) {
    checked = true;
    const CommandLine& command = *CommandLine::ForCurrentProcess();
    enabled = command.HasSwitch(switches::kEnableMonitorProfile);
  }
  return enabled;
}
