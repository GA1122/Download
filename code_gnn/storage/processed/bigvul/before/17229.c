void AddFormFactorSpecificCommandLineArguments() {
  if (GetFormFactorHint() == FORM_FACTOR_DESKTOP) {
    return;
  }

  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  command_line->AppendSwitch(switches::kEnableOverlayScrollbar);
}
