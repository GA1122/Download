bool DevToolsWindow::HasFiredBeforeUnloadEventForDevToolsBrowser(
    Browser* browser) {
  DCHECK(browser->is_devtools());
  if (browser->tab_strip_model()->empty())
    return true;
  WebContents* contents =
      browser->tab_strip_model()->GetWebContentsAt(0);
  DevToolsWindow* window = AsDevToolsWindow(contents);
  if (!window)
    return false;
  return window->intercepted_page_beforeunload_;
}
