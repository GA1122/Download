void BlobStorageContext::OnEnoughSpaceForTransport(
    const std::string& uuid,
    std::vector<BlobMemoryController::FileCreationInfo> files,
    bool success) {
  if (!success) {
    CancelBuildingBlob(uuid, BlobStatus::ERR_OUT_OF_MEMORY);
    return;
  }
  BlobEntry* entry = registry_.GetEntry(uuid);
  if (!entry || !entry->building_state_.get())
    return;
  BlobEntry::BuildingState& building_state = *entry->building_state_;
  DCHECK(!building_state.transport_quota_request);
  DCHECK(building_state.transport_items_present);

  entry->set_status(BlobStatus::PENDING_TRANSPORT);
  RequestTransport(entry, std::move(files));

  if (entry->CanFinishBuilding())
    FinishBuilding(entry);
}
