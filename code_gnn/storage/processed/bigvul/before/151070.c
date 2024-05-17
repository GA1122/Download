BrowserWindow* DevToolsWindow::GetInspectedBrowserWindow() {
  Browser* browser = NULL;
  int tab;
  return FindInspectedBrowserAndTabIndex(GetInspectedWebContents(),
                                         &browser, &tab) ?
      browser->window() : NULL;
}
