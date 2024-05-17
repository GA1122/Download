bool ShouldReloadCrashedTab(WebContents* contents) {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  return (command_line.HasSwitch(switches::kReloadKilledTabs) &&
      contents->IsCrashed());
}
