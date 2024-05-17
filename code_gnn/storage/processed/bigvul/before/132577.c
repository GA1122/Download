void WebKitTestController::OnCloseRemainingWindows() {
  DevToolsAgentHost::DetachAllClients();
  std::vector<Shell*> open_windows(Shell::windows());
  Shell* devtools_shell = devtools_frontend_ ?
      devtools_frontend_->frontend_shell() : NULL;
  for (size_t i = 0; i < open_windows.size(); ++i) {
    if (open_windows[i] != main_window_ && open_windows[i] != devtools_shell)
      open_windows[i]->Close();
  }
  base::MessageLoop::current()->RunUntilIdle();
}
