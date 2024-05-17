BrowserWindow* BrowserWindow::CreateBrowserWindow(Browser* browser) {
  BrowserWindowGtk* browser_window_gtk = new BrowserWindowGtk(browser);
  browser_window_gtk->Init();
  return browser_window_gtk;
}
