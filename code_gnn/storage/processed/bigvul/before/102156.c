void SyncManager::LogUnsyncedItems(int level) const {
  std::vector<int64> unsynced_handles;
  sync_api::ReadTransaction trans(FROM_HERE, GetUserShare());
  trans.GetWrappedTrans()->directory()->GetUnsyncedMetaHandles(
      trans.GetWrappedTrans(), &unsynced_handles);

  for (std::vector<int64>::const_iterator it = unsynced_handles.begin();
       it != unsynced_handles.end(); ++it) {
    ReadNode node(&trans);
    if (node.InitByIdLookup(*it)) {
      scoped_ptr<DictionaryValue> value(node.GetDetailsAsValue());
      std::string info;
      base::JSONWriter::Write(value.get(), true, &info);
      VLOG(level) << info;
    }
  }
}
