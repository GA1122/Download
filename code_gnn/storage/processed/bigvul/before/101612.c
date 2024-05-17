BrowserWindow* Browser::CreateBrowserWindow() {
  if (type_ == TYPE_PANEL &&
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnablePanels))
    return PanelManager::GetInstance()->CreatePanel(this);

  return BrowserWindow::CreateBrowserWindow(this);
}
