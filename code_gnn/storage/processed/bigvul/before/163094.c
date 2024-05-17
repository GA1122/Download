void BlobStorageContext::ClearAndFreeMemory(BlobEntry* entry) {
  if (entry->building_state_)
    entry->building_state_->CancelRequests();
  entry->ClearItems();
  entry->ClearOffsets();
  entry->set_size(0);
}
