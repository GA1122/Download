bool ConflictResolver::ResolveConflicts(syncable::WriteTransaction* trans,
                                        const Cryptographer* cryptographer,
                                        const ConflictProgress& progress,
                                        sessions::StatusController* status) {
  bool forward_progress = false;
  set<Id>::const_iterator conflicting_item_it;
  set<Id> processed_items;
  for (conflicting_item_it = progress.SimpleConflictingItemsBegin();
       conflicting_item_it != progress.SimpleConflictingItemsEnd();
       ++conflicting_item_it) {
    Id id = *conflicting_item_it;
    if (processed_items.count(id) > 0)
      continue;

    list<Id> predecessors;
    Id prev_id = id;
    do {
      predecessors.push_back(prev_id);
      Entry entry(trans, syncable::GET_BY_ID, prev_id);
      CHECK(entry.good());
      Id new_prev_id = entry.Get(syncable::PREV_ID);
      if (new_prev_id == prev_id)
        break;
      prev_id = new_prev_id;
    } while (processed_items.count(prev_id) == 0 &&
             progress.HasSimpleConflictItem(prev_id));   
    while (!predecessors.empty()) {
      id = predecessors.back();
      predecessors.pop_back();
      switch (ProcessSimpleConflict(trans, id, cryptographer, status)) {
        case NO_SYNC_PROGRESS:
          break;
        case SYNC_PROGRESS:
          forward_progress = true;
          break;
      }
      processed_items.insert(id);
    }
  }
  return forward_progress;
}
