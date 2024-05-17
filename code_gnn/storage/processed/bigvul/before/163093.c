void BlobStorageContext::CancelBuildingBlobInternal(BlobEntry* entry,
                                                    BlobStatus reason) {
  DCHECK(entry);
  DCHECK(BlobStatusIsError(reason));
  TransportAllowedCallback transport_allowed_callback;
  if (entry->building_state_ &&
      entry->building_state_->transport_allowed_callback) {
    transport_allowed_callback =
        entry->building_state_->transport_allowed_callback;
    entry->building_state_->transport_allowed_callback.Reset();
  }
  if (entry->building_state_ &&
      entry->status() == BlobStatus::PENDING_CONSTRUCTION) {
    auto runner = base::ThreadTaskRunnerHandle::Get();
    for (const auto& callback : entry->building_state_->build_started_callbacks)
      runner->PostTask(FROM_HERE, base::BindOnce(callback, reason));
  }
  ClearAndFreeMemory(entry);
  entry->set_status(reason);
  if (transport_allowed_callback) {
    transport_allowed_callback.Run(
        reason, std::vector<BlobMemoryController::FileCreationInfo>());
  }
  FinishBuilding(entry);
}
