void SyncerUtil::UpdateLocalDataFromServerData(
    syncable::WriteTransaction* trans,
    syncable::MutableEntry* entry) {
  DCHECK(!entry->Get(IS_UNSYNCED));
  DCHECK(entry->Get(IS_UNAPPLIED_UPDATE));

  VLOG(2) << "Updating entry : " << *entry;
  entry->Put(SPECIFICS, entry->Get(SERVER_SPECIFICS));
  entry->Put(IS_DIR, entry->Get(SERVER_IS_DIR));
  if (entry->Get(SERVER_IS_DEL)) {
    entry->Put(IS_DEL, true);
  } else {
    entry->Put(NON_UNIQUE_NAME, entry->Get(SERVER_NON_UNIQUE_NAME));
    entry->Put(PARENT_ID, entry->Get(SERVER_PARENT_ID));
    CHECK(entry->Put(IS_DEL, false));
    Id new_predecessor =
        entry->ComputePrevIdFromServerPosition(entry->Get(SERVER_PARENT_ID));
    CHECK(entry->PutPredecessor(new_predecessor))
        << " Illegal predecessor after converting from server position.";
  }

  entry->Put(CTIME, entry->Get(SERVER_CTIME));
  entry->Put(MTIME, entry->Get(SERVER_MTIME));
  entry->Put(BASE_VERSION, entry->Get(SERVER_VERSION));
  entry->Put(IS_DEL, entry->Get(SERVER_IS_DEL));
  entry->Put(IS_UNAPPLIED_UPDATE, false);
}
