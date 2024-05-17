bool ExtensionTabUtil::GetDefaultTab(Browser* browser, TabContents** contents,
                                     int* tab_id) {
  DCHECK(browser);
  DCHECK(contents);
  DCHECK(tab_id);

  *contents = browser->tabstrip_model()->GetSelectedTabContents();
  if (*contents) {
    if (tab_id)
      *tab_id = ExtensionTabUtil::GetTabId(*contents);
    return true;
  }

  return false;
}
