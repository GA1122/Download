void ExtensionService::GetSyncDataListHelper(
    const ExtensionList& extensions,
    ExtensionFilter filter,
    std::vector<ExtensionSyncData>* sync_data_list) const {
  for (ExtensionList::const_iterator it = extensions.begin();
       it != extensions.end(); ++it) {
    const Extension& extension = **it;
    if ((*filter)(extension)) {
      sync_data_list->push_back(GetSyncDataHelper(extension));
    }
  }
}
