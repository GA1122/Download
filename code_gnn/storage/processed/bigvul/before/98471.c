bool AeroPeekManager::Enabled() {
  const CommandLine* command_line = CommandLine::ForCurrentProcess();
  return win_util::GetWinVersion() >= win_util::WINVERSION_WIN7 &&
      win_util::ShouldUseVistaFrame() &&
      !command_line->HasSwitch(switches::kApp) &&
      command_line->HasSwitch(switches::kEnableAeroPeekTabs);
}
