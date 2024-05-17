void SyncerUtil::ChangeEntryIDAndUpdateChildren(
    syncable::WriteTransaction* trans,
    syncable::MutableEntry* entry,
    const syncable::Id& new_id,
    syncable::Directory::ChildHandles* children) {
  syncable::Id old_id = entry->Get(ID);
  if (!entry->Put(ID, new_id)) {
    Entry old_entry(trans, GET_BY_ID, new_id);
    CHECK(old_entry.good());
    LOG(FATAL) << "Attempt to change ID to " << new_id
               << " conflicts with existing entry.\n\n"
               << *entry << "\n\n" << old_entry;
  }
  if (entry->Get(IS_DIR)) {
    trans->directory()->GetChildHandlesById(trans, old_id, children);
    Directory::ChildHandles::iterator i = children->begin();
    while (i != children->end()) {
      MutableEntry child_entry(trans, GET_BY_HANDLE, *i++);
      CHECK(child_entry.good());
      child_entry.PutParentIdPropertyOnly(new_id);
    }
  }
  if (entry->Get(PREV_ID) == entry->Get(NEXT_ID) &&
      entry->Get(PREV_ID) == old_id) {
    entry->Put(NEXT_ID, new_id);
    entry->Put(PREV_ID, new_id);
  } else {
    entry->PutPredecessor(entry->Get(PREV_ID));
  }
}
