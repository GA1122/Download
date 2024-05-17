VerifyCommitResult SyncerUtil::ValidateCommitEntry(
    syncable::Entry* entry) {
  syncable::Id id = entry->Get(ID);
  if (id == entry->Get(PARENT_ID)) {
    CHECK(id.IsRoot()) << "Non-root item is self parenting." << *entry;
    LOG(ERROR) << "Root item became unsynced " << *entry;
    return VERIFY_UNSYNCABLE;
  }
  if (entry->IsRoot()) {
    LOG(ERROR) << "Permanent item became unsynced " << *entry;
    return VERIFY_UNSYNCABLE;
  }
  if (entry->Get(IS_DEL) && !entry->Get(ID).ServerKnows()) {
    return VERIFY_UNSYNCABLE;
  }
  return VERIFY_OK;
}
