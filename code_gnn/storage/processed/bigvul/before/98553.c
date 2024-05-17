DictionaryValue* ExtensionTabUtil::CreateTabValue(
    const TabContents* contents) {
  for (BrowserList::const_iterator it = BrowserList::begin();
      it != BrowserList::end(); ++it) {
    TabStripModel* tab_strip = (*it)->tabstrip_model();
    int tab_index = tab_strip->GetIndexOfTabContents(contents);
    if (tab_index != -1) {
      return ExtensionTabUtil::CreateTabValue(contents, tab_strip, tab_index);
    }
  }

  return ExtensionTabUtil::CreateTabValue(contents, NULL, -1);
}
