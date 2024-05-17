void BlobStorageContext::OnDependentBlobFinished(
    const std::string& owning_blob_uuid,
    BlobStatus status) {
  BlobEntry* entry = registry_.GetEntry(owning_blob_uuid);
  if (!entry || !entry->building_state_)
    return;

  if (BlobStatusIsError(status)) {
    DCHECK_NE(BlobStatus::ERR_BLOB_DEREFERENCED_WHILE_BUILDING, status)
        << "Referenced blob should never be dereferenced while we "
        << "are depending on it, as our system holds a handle.";
    CancelBuildingBlobInternal(entry, BlobStatus::ERR_REFERENCED_BLOB_BROKEN);
    return;
  }
  DCHECK_GT(entry->building_state_->num_building_dependent_blobs, 0u);
  --entry->building_state_->num_building_dependent_blobs;

  if (entry->CanFinishBuilding())
    FinishBuilding(entry);
}
