bool IsAutoReloadEnabled() {
  const base::CommandLine& browser_command_line =
      *base::CommandLine::ForCurrentProcess();
  if (browser_command_line.HasSwitch(switches::kEnableOfflineAutoReload))
    return true;
  if (browser_command_line.HasSwitch(switches::kDisableOfflineAutoReload))
    return false;
  return true;
}
