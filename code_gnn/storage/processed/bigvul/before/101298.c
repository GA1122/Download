void SyncerUtil::SplitServerInformationIntoNewEntry(
    syncable::WriteTransaction* trans,
    syncable::MutableEntry* entry) {
  syncable::Id id = entry->Get(ID);
  ChangeEntryIDAndUpdateChildren(trans, entry, trans->directory()->NextId());
  entry->Put(BASE_VERSION, 0);

  MutableEntry new_entry(trans, CREATE_NEW_UPDATE_ITEM, id);
  CopyServerFields(entry, &new_entry);
  ClearServerData(entry);

  VLOG(1) << "Splitting server information, local entry: " << *entry
          << " server entry: " << new_entry;
}
