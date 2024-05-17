Browser* FindBrowser(content::WebContents* web_contents) {
  for (auto* browser : *BrowserList::GetInstance()) {
    int tab_index = browser->tab_strip_model()->GetIndexOfWebContents(
        web_contents);
    if (tab_index != TabStripModel::kNoTab)
      return browser;
  }
  return NULL;
}
