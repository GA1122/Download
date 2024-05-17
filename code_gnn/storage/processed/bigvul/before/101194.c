int64 BuildCommitCommand::FindAnchorPosition(syncable::IdField direction,
                                             const syncable::Entry& entry) {
  Id next_id = entry.Get(direction);
  while (!next_id.IsRoot()) {
    Entry next_entry(entry.trans(),
                     syncable::GET_BY_ID,
                     next_id);
    if (!next_entry.Get(IS_UNSYNCED) && !next_entry.Get(IS_UNAPPLIED_UPDATE)) {
      return next_entry.Get(SERVER_POSITION_IN_PARENT);
    }
    next_id = next_entry.Get(direction);
  }
  return
      direction == syncable::PREV_ID ?
      GetFirstPosition() : GetLastPosition();
}
