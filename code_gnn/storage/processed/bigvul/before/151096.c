void DevToolsWindow::OpenInNewTab(const std::string& url) {
  content::OpenURLParams params(GURL(url), content::Referrer(),
                                WindowOpenDisposition::NEW_FOREGROUND_TAB,
                                ui::PAGE_TRANSITION_LINK, false);
  WebContents* inspected_web_contents = GetInspectedWebContents();
  if (!inspected_web_contents || !inspected_web_contents->OpenURL(params)) {
    chrome::ScopedTabbedBrowserDisplayer displayer(profile_);
    chrome::AddSelectedTabWithURL(displayer.browser(), GURL(url),
                                  ui::PAGE_TRANSITION_LINK);
  }
}
