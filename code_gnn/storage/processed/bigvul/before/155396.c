bool IsAutoReloadVisibleOnlyEnabled() {
  const base::CommandLine& browser_command_line =
      *base::CommandLine::ForCurrentProcess();
  if (browser_command_line.HasSwitch(
      switches::kEnableOfflineAutoReloadVisibleOnly)) {
    return true;
  }
  if (browser_command_line.HasSwitch(
      switches::kDisableOfflineAutoReloadVisibleOnly)) {
    return false;
  }
  return true;
}
