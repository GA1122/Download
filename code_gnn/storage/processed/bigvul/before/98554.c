DictionaryValue* ExtensionTabUtil::CreateTabValue(
    const TabContents* contents, TabStripModel* tab_strip, int tab_index) {
  DictionaryValue* result = new DictionaryValue();
  result->SetInteger(keys::kIdKey, ExtensionTabUtil::GetTabId(contents));
  result->SetInteger(keys::kIndexKey, tab_index);
  result->SetInteger(keys::kWindowIdKey,
                     ExtensionTabUtil::GetWindowIdOfTab(contents));
  result->SetString(keys::kUrlKey, contents->GetURL().spec());
  result->SetString(keys::kStatusKey, GetTabStatusText(contents->is_loading()));
  result->SetBoolean(keys::kSelectedKey,
                     tab_strip && tab_index == tab_strip->selected_index());
  result->SetString(keys::kTitleKey, UTF16ToWide(contents->GetTitle()));
  result->SetBoolean(keys::kIncognitoKey,
                     contents->profile()->IsOffTheRecord());

  if (!contents->is_loading()) {
    NavigationEntry* entry = contents->controller().GetActiveEntry();
    if (entry) {
      if (entry->favicon().is_valid())
        result->SetString(keys::kFavIconUrlKey, entry->favicon().url().spec());
    }
  }

  return result;
}
