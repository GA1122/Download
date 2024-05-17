bool ChromeContentBrowserClient::IsFastShutdownPossible() {
  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  return !browser_command_line.HasSwitch(switches::kChromeFrame);
}
