void BrowserEventRouter::TabChangedAt(WebContents* contents,
                                      int index,
                                      TabChangeType change_type) {
  TabUpdated(contents, false);
}
