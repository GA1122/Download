bool QueryManager::AddPendingTransferQuery(
    Query* query,
    base::subtle::Atomic32 submit_count) {
  DCHECK(query);
  DCHECK(!query->IsDeleted());
  if (!RemovePendingQuery(query)) {
    return false;
  }
  query->MarkAsPending(submit_count);
  pending_transfer_queries_.push_back(query);
  return true;
}
