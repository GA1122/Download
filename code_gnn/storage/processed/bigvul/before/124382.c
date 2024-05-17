DictionaryValue* BrowserEventRouter::TabEntry::UpdateLoadState(
    const WebContents* contents) {
  if (!complete_waiting_on_load_ || contents->IsLoading())
    return NULL;

  complete_waiting_on_load_ = false;
  DictionaryValue* changed_properties = new DictionaryValue();
  changed_properties->SetString(tab_keys::kStatusKey,
      tab_keys::kStatusValueComplete);
  return changed_properties;
}
