void SyncerUtil::AddPredecessorsThenItem(
    syncable::BaseTransaction* trans,
    syncable::Entry* item,
    syncable::IndexedBitField inclusion_filter,
    syncable::MetahandleSet* inserted_items,
    std::vector<syncable::Id>* commit_ids) {
  size_t initial_size = commit_ids->size();
  if (!AddItemThenPredecessors(trans, item, inclusion_filter, inserted_items,
                               commit_ids))
    return;
  std::reverse(commit_ids->begin() + initial_size, commit_ids->end());
}
