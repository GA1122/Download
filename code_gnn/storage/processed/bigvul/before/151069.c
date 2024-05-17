InfoBarService* DevToolsWindow::GetInfoBarService() {
  return is_docked_ ?
      InfoBarService::FromWebContents(GetInspectedWebContents()) :
      InfoBarService::FromWebContents(main_web_contents_);
}
