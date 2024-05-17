int SyncerUtil::GetUnsyncedEntries(syncable::BaseTransaction* trans,
                                   std::vector<int64> *handles) {
  trans->directory()->GetUnsyncedMetaHandles(trans, handles);
  VLOG_IF(1, !handles->empty()) << "Have " << handles->size()
                                << " unsynced items.";
  return handles->size();
}
