int ExtensionTabUtil::GetWindowIdOfTabStripModel(
    const TabStripModel* tab_strip_model) {
  for (BrowserList::const_iterator it = BrowserList::begin();
       it != BrowserList::end(); ++it) {
    if ((*it)->tab_strip_model() == tab_strip_model)
      return GetWindowId(*it);
  }
  return -1;
}
