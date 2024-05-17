void BlobStorageContext::FinishBuilding(BlobEntry* entry) {
  DCHECK(entry);

  BlobStatus status = entry->status_;
  DCHECK_NE(BlobStatus::DONE, status);

  bool error = BlobStatusIsError(status);
  UMA_HISTOGRAM_BOOLEAN("Storage.Blob.Broken", error);
  if (error) {
    UMA_HISTOGRAM_ENUMERATION("Storage.Blob.BrokenReason",
                              static_cast<int>(status),
                              (static_cast<int>(BlobStatus::LAST_ERROR) + 1));
  }

  if (BlobStatusIsPending(entry->status_)) {
    for (const ItemCopyEntry& copy : entry->building_state_->copies) {
      size_t dest_size = static_cast<size_t>(copy.dest_item->item()->length());
      DataElement::Type dest_type = copy.dest_item->item()->type();
      switch (copy.source_item->item()->type()) {
        case DataElement::TYPE_BYTES: {
          DCHECK_EQ(dest_type, DataElement::TYPE_BYTES_DESCRIPTION);
          const char* src_data =
              copy.source_item->item()->bytes() + copy.source_item_offset;
          copy.dest_item->item()->item_->SetToBytes(src_data, dest_size);
          break;
        }
        case DataElement::TYPE_FILE: {
          if (dest_type == DataElement::TYPE_BYTES_DESCRIPTION)
            copy.dest_item->set_memory_allocation(nullptr);

          const DataElement& source_element =
              copy.source_item->item()->data_element();
          std::unique_ptr<DataElement> new_element(new DataElement());
          new_element->SetToFilePathRange(
              source_element.path(),
              source_element.offset() + copy.source_item_offset, dest_size,
              source_element.expected_modification_time());
          scoped_refptr<BlobDataItem> new_item(new BlobDataItem(
              std::move(new_element), copy.source_item->item()->data_handle_));
          copy.dest_item->set_item(std::move(new_item));
          break;
        }
        case DataElement::TYPE_UNKNOWN:
        case DataElement::TYPE_BLOB:
        case DataElement::TYPE_BYTES_DESCRIPTION:
        case DataElement::TYPE_FILE_FILESYSTEM:
        case DataElement::TYPE_DISK_CACHE_ENTRY:
        case DataElement::TYPE_DATA_PIPE:
          NOTREACHED();
          break;
      }
      copy.dest_item->set_state(ShareableBlobDataItem::POPULATED_WITH_QUOTA);
    }

    entry->set_status(BlobStatus::DONE);
  }

  std::vector<BlobStatusCallback> callbacks;
  if (entry->building_state_.get()) {
    std::swap(callbacks, entry->building_state_->build_completion_callbacks);
    entry->building_state_.reset();
  }

  memory_controller_.NotifyMemoryItemsUsed(entry->items());

  auto runner = base::ThreadTaskRunnerHandle::Get();
  for (const auto& callback : callbacks)
    runner->PostTask(FROM_HERE, base::Bind(callback, entry->status()));

  for (const auto& shareable_item : entry->items()) {
    DCHECK_NE(DataElement::TYPE_BYTES_DESCRIPTION,
              shareable_item->item()->type());
    DCHECK(shareable_item->IsPopulated()) << shareable_item->state();
  }
}
