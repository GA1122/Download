int64 SessionModelAssociator::GetSyncIdFromSessionTag(const std::string& tag) {
  DCHECK(CalledOnValidThread());
  sync_api::ReadTransaction trans(FROM_HERE, sync_service_->GetUserShare());
  sync_api::ReadNode node(&trans);
  if (!node.InitByClientTagLookup(syncable::SESSIONS, tag))
    return sync_api::kInvalidId;
  return node.GetId();
}
