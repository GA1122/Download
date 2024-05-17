void SyncerUtil::MarkDeletedChildrenSynced(
    const syncable::ScopedDirLookup &dir,
    std::set<syncable::Id>* deleted_folders) {
  if (deleted_folders->empty())
    return;
  Directory::UnsyncedMetaHandles handles;
  {
    ReadTransaction trans(FROM_HERE, dir);
    dir->GetUnsyncedMetaHandles(&trans, &handles);
  }
  if (handles.empty())
    return;
  Directory::UnsyncedMetaHandles::iterator it;
  for (it = handles.begin() ; it != handles.end() ; ++it) {
    WriteTransaction trans(FROM_HERE, SYNCER, dir);
    MutableEntry entry(&trans, GET_BY_HANDLE, *it);
    if (!entry.Get(IS_UNSYNCED) || !entry.Get(IS_DEL))
      continue;
    syncable::Id id = entry.Get(PARENT_ID);
    while (id != trans.root_id()) {
      if (deleted_folders->find(id) != deleted_folders->end()) {
        entry.Put(IS_UNSYNCED, false);
        break;
      }
      Entry parent(&trans, GET_BY_ID, id);
      if (!parent.good() || !parent.Get(IS_DEL))
        break;
      id = parent.Get(PARENT_ID);
    }
  }
}
