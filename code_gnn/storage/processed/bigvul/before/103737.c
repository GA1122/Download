bool DevToolsClient::shouldHideScriptsPanel() {
  CommandLine* cmd = CommandLine::ForCurrentProcess();
  return cmd->HasSwitch(switches::kRemoteShellPort);
}
