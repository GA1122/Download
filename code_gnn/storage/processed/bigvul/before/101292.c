void SyncerUtil::ChangeEntryIDAndUpdateChildren(
    syncable::WriteTransaction* trans,
    syncable::MutableEntry* entry,
    const syncable::Id& new_id) {
  syncable::Directory::ChildHandles children;
  ChangeEntryIDAndUpdateChildren(trans, entry, new_id, &children);
}
