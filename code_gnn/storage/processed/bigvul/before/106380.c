void OfflineLoadPage::CommandReceived(const std::string& cmd) {
  std::string command(cmd);
  if (command.length() > 1 && command[0] == '"') {
    command = command.substr(1, command.length() - 2);
  }
  if (command == "proceed") {
    Proceed();
  } else if (command == "dontproceed") {
    DontProceed();
  } else if (command == "open_network_settings") {
    Browser* browser = BrowserList::GetLastActive();
    DCHECK(browser);
    browser->ShowOptionsTab(chrome::kInternetOptionsSubPage);
  } else if (command == "open_activate_broadband") {
    Browser* browser = BrowserList::GetLastActive();
    DCHECK(browser);
    browser->OpenMobilePlanTabAndActivate();
  } else {
    LOG(WARNING) << "Unknown command:" << cmd;
  }
}
