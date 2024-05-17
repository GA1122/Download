void Browser::OnWindowActivated() {
  WebContents* contents = chrome::GetActiveWebContents(this);
  if (contents && ShouldReloadCrashedTab(contents))
    chrome::Reload(this, CURRENT_TAB);
}
