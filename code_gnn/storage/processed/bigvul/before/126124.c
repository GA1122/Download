void BrowserEventRouter::TabChangedAt(TabContents* contents,
                                      int index,
                                      TabChangeType change_type) {
  TabUpdated(contents->web_contents(), false);
}
