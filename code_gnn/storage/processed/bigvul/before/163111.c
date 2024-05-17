void BlobStorageContext::RequestTransport(
    BlobEntry* entry,
    std::vector<BlobMemoryController::FileCreationInfo> files) {
  BlobEntry::BuildingState* building_state = entry->building_state_.get();
  if (building_state->transport_allowed_callback) {
    base::ResetAndReturn(&building_state->transport_allowed_callback)
        .Run(BlobStatus::PENDING_TRANSPORT, std::move(files));
    return;
  }
  DCHECK(files.empty());
  NotifyTransportCompleteInternal(entry);
}
