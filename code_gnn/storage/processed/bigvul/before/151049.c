void DevToolsWindow::ActivateWindow() {
  if (life_stage_ != kLoadCompleted)
    return;
  if (is_docked_ && GetInspectedBrowserWindow())
    main_web_contents_->Focus();
  else if (!is_docked_ && !browser_->window()->IsActive())
    browser_->window()->Activate();
}
