DictionaryValue* CreateCookieStoreValue(Profile* profile,
                                        ListValue* tab_ids) {
  DCHECK(profile);
  DCHECK(tab_ids);
  DictionaryValue* result = new DictionaryValue();
  result->SetString(keys::kIdKey, GetStoreIdFromProfile(profile));
  result->Set(keys::kTabIdsKey, tab_ids);
  return result;
}
