bool SyncerUtil::AddItemThenPredecessors(
    syncable::BaseTransaction* trans,
    syncable::Entry* item,
    syncable::IndexedBitField inclusion_filter,
    syncable::MetahandleSet* inserted_items,
    std::vector<syncable::Id>* commit_ids) {

  if (!inserted_items->insert(item->Get(META_HANDLE)).second)
    return false;
  commit_ids->push_back(item->Get(ID));
  if (item->Get(IS_DEL))
    return true;   

  Id prev_id = item->Get(PREV_ID);
  while (!prev_id.IsRoot()) {
    Entry prev(trans, GET_BY_ID, prev_id);
    CHECK(prev.good()) << "Bad id when walking predecessors.";
    if (!prev.Get(inclusion_filter))
      break;
    if (!inserted_items->insert(prev.Get(META_HANDLE)).second)
      break;
    commit_ids->push_back(prev_id);
    prev_id = prev.Get(PREV_ID);
  }
  return true;
}
