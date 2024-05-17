std::unique_ptr<BlobDataHandle> BlobStorageContext::BuildBlobInternal(
    BlobEntry* entry,
    const BlobDataBuilder& content,
    const TransportAllowedCallback& transport_allowed_callback) {
  BlobFlattener flattener(content, entry, &registry_);

  DCHECK(!flattener.contains_unpopulated_transport_items ||
         transport_allowed_callback)
      << "If we have pending unpopulated content then a callback is required";

  DCHECK(flattener.total_size == 0 ||
         flattener.total_size == entry->total_size());
  entry->set_size(flattener.total_size);
  entry->set_status(flattener.status);
  std::unique_ptr<BlobDataHandle> handle = CreateHandle(content.uuid_, entry);

  UMA_HISTOGRAM_COUNTS_1M("Storage.Blob.ItemCount", entry->items().size());
  UMA_HISTOGRAM_COUNTS_1M("Storage.Blob.TotalSize",
                          flattener.total_memory_size / 1024);

  uint64_t total_memory_needed =
      flattener.copy_quota_needed +
      (flattener.transport_quota_type == TransportQuotaType::MEMORY
           ? flattener.transport_quota_needed
           : 0);
  UMA_HISTOGRAM_COUNTS_1M("Storage.Blob.TotalUnsharedSize",
                          total_memory_needed / 1024);

  size_t num_building_dependent_blobs = 0;
  std::vector<std::unique_ptr<BlobDataHandle>> dependent_blobs;
  for (const std::pair<std::string, BlobEntry*>& pending_blob :
       flattener.dependent_blobs) {
    dependent_blobs.push_back(
        CreateHandle(pending_blob.first, pending_blob.second));
    if (BlobStatusIsPending(pending_blob.second->status())) {
      pending_blob.second->building_state_->build_completion_callbacks
          .push_back(base::Bind(&BlobStorageContext::OnDependentBlobFinished,
                                ptr_factory_.GetWeakPtr(), content.uuid_));
      num_building_dependent_blobs++;
    }
  }

  auto previous_building_state = std::move(entry->building_state_);
  entry->set_building_state(base::MakeUnique<BlobEntry::BuildingState>(
      !flattener.pending_transport_items.empty(), transport_allowed_callback,
      num_building_dependent_blobs));
  BlobEntry::BuildingState* building_state = entry->building_state_.get();
  std::swap(building_state->copies, flattener.copies);
  std::swap(building_state->dependent_blobs, dependent_blobs);
  std::swap(building_state->transport_items, flattener.transport_items);
  if (previous_building_state) {
    DCHECK(!previous_building_state->transport_items_present);
    DCHECK(!previous_building_state->transport_allowed_callback);
    DCHECK(previous_building_state->transport_items.empty());
    DCHECK(previous_building_state->dependent_blobs.empty());
    DCHECK(previous_building_state->copies.empty());
    std::swap(building_state->build_completion_callbacks,
              previous_building_state->build_completion_callbacks);
    auto runner = base::ThreadTaskRunnerHandle::Get();
    for (const auto& callback :
         previous_building_state->build_started_callbacks)
      runner->PostTask(FROM_HERE, base::BindOnce(callback, entry->status()));
  }

  if (BlobStatusIsError(flattener.status)) {
    CancelBuildingBlobInternal(entry, flattener.status);
    return handle;
  }

  if (!memory_controller_.CanReserveQuota(flattener.copy_quota_needed +
                                          flattener.transport_quota_needed)) {
    CancelBuildingBlobInternal(entry, BlobStatus::ERR_OUT_OF_MEMORY);
    return handle;
  }

  if (flattener.copy_quota_needed > 0) {
    base::WeakPtr<QuotaAllocationTask> pending_request =
        memory_controller_.ReserveMemoryQuota(
            std::move(flattener.pending_copy_items),
            base::Bind(&BlobStorageContext::OnEnoughSpaceForCopies,
                       ptr_factory_.GetWeakPtr(), content.uuid_));
    if (entry->building_state_)
      entry->building_state_->copy_quota_request = std::move(pending_request);
  }

  if (flattener.transport_quota_needed > 0) {
    base::WeakPtr<QuotaAllocationTask> pending_request;

    switch (flattener.transport_quota_type) {
      case TransportQuotaType::MEMORY: {
        std::vector<BlobMemoryController::FileCreationInfo> empty_files;
        pending_request = memory_controller_.ReserveMemoryQuota(
            std::move(flattener.pending_transport_items),
            base::Bind(&BlobStorageContext::OnEnoughSpaceForTransport,
                       ptr_factory_.GetWeakPtr(), content.uuid_,
                       base::Passed(&empty_files)));
        break;
      }
      case TransportQuotaType::FILE:
        pending_request = memory_controller_.ReserveFileQuota(
            std::move(flattener.pending_transport_items),
            base::Bind(&BlobStorageContext::OnEnoughSpaceForTransport,
                       ptr_factory_.GetWeakPtr(), content.uuid_));
        break;
    }

    if (entry->building_state_) {
      entry->building_state_->transport_quota_request =
          std::move(pending_request);
    }
  }

  if (entry->CanFinishBuilding())
    FinishBuilding(entry);

  return handle;
}
