WebContents* BrowserView::GetActiveWebContents() const {
  return chrome::GetActiveWebContents(browser_.get());
}
