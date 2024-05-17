void TestingAutomationProvider::ExecuteBrowserCommandAsync(int handle,
                                                           int command,
                                                           bool* success) {
  *success = false;
  if (!browser_tracker_->ContainsHandle(handle)) {
    LOG(WARNING) << "Browser tracker does not contain handle: " << handle;
    return;
  }
  Browser* browser = browser_tracker_->GetResource(handle);
  if (!browser->command_updater()->SupportsCommand(command)) {
    LOG(WARNING) << "Browser does not support command: " << command;
    return;
  }
  if (!browser->command_updater()->IsCommandEnabled(command)) {
    LOG(WARNING) << "Browser command not enabled: " << command;
    return;
  }
  browser->ExecuteCommand(command);
  *success = true;
}
