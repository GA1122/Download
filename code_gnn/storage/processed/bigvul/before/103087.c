BrowserWindow* Browser::CreateBrowserWindow() {
  if (type() == Browser::TYPE_APP_PANEL &&
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnablePanels))
    return PanelManager::GetInstance()->CreatePanel(this);

  return BrowserWindow::CreateBrowserWindow(this);
}
