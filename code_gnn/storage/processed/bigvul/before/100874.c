int ParseRemovalMask(base::DictionaryValue* value) {
  int GetRemovalMask = 0;
  if (DataRemovalRequested(value, extension_clear_api_constants::kAppCacheKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_APPCACHE;
  if (DataRemovalRequested(value, extension_clear_api_constants::kCacheKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_CACHE;
  if (DataRemovalRequested(value, extension_clear_api_constants::kCookiesKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_COOKIES;
  if (DataRemovalRequested(value, extension_clear_api_constants::kDownloadsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_DOWNLOADS;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_FILE_SYSTEMS;
  if (DataRemovalRequested(value, extension_clear_api_constants::kFormDataKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_FORM_DATA;
  if (DataRemovalRequested(value, extension_clear_api_constants::kHistoryKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_HISTORY;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_INDEXEDDB;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_LOCAL_STORAGE;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_PLUGIN_DATA;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_PASSWORDS;
  if (DataRemovalRequested(value, extension_clear_api_constants::kPasswordsKey))
    GetRemovalMask |= BrowsingDataRemover::REMOVE_WEBSQL;

  return GetRemovalMask;
}
