DictionaryValue* SessionWindowToValue(
    const sync_pb::SessionWindow& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_INT32(window_id);
  SET_INT32(selected_tab_index);
  SET_INT32_REP(tab);
  SET_ENUM(browser_type, GetBrowserTypeString);
  return value;
}
