DictionaryValue* TabNavigationToValue(
    const sync_pb::TabNavigation& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(virtual_url);
  SET_STR(referrer);
  SET_STR(title);
  SET_STR(state);
  SET_ENUM(page_transition, GetPageTransitionString);
  SET_ENUM(navigation_qualifier, GetPageTransitionQualifierString);
  SET_INT32(unique_id);
  SET_INT64(timestamp);
  return value;
}
