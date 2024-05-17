TabContents* BrowserView::GetActiveTabContents() const {
  return chrome::GetActiveTabContents(browser_.get());
}
