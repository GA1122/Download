BlobStorageContext::BlobSlice::BlobSlice(const BlobEntry& source,
                                         uint64_t slice_offset,
                                         uint64_t slice_size) {
  const auto& source_items = source.items();
  const auto& offsets = source.offsets();
  DCHECK_LE(slice_offset + slice_size, source.total_size());
  size_t item_index =
      std::upper_bound(offsets.begin(), offsets.end(), slice_offset) -
      offsets.begin();
  uint64_t item_offset =
      item_index == 0 ? slice_offset : slice_offset - offsets[item_index - 1];
  size_t num_items = source_items.size();

  size_t first_item_index = item_index;

  for (uint64_t total_sliced = 0;
       item_index < num_items && total_sliced < slice_size; item_index++) {
    const scoped_refptr<BlobDataItem>& source_item =
        source_items[item_index]->item();
    uint64_t source_length = source_item->length();
    DataElement::Type type = source_item->type();
    DCHECK_NE(source_length, std::numeric_limits<uint64_t>::max());
    DCHECK_NE(source_length, 0ull);

    uint64_t read_size =
        std::min(source_length - item_offset, slice_size - total_sliced);
    total_sliced += read_size;

    bool reusing_blob_item = (read_size == source_length);
    UMA_HISTOGRAM_BOOLEAN("Storage.Blob.ReusedItem", reusing_blob_item);
    if (reusing_blob_item) {
      dest_items.push_back(source_items[item_index]);
      if (IsBytes(type)) {
        total_memory_size += source_length;
      }
      continue;
    }

    scoped_refptr<BlobDataItem> data_item;
    ShareableBlobDataItem::State state =
        ShareableBlobDataItem::POPULATED_WITHOUT_QUOTA;
    switch (type) {
      case DataElement::TYPE_BYTES_DESCRIPTION:
      case DataElement::TYPE_BYTES: {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.BlobSlice.Bytes",
                                read_size / 1024);
        if (item_index == first_item_index) {
          first_item_slice_offset = item_offset;
          first_source_item = source_items[item_index];
        } else {
          last_source_item = source_items[item_index];
        }
        copying_memory_size += read_size;
        total_memory_size += read_size;
        std::unique_ptr<DataElement> element(new DataElement());
        element->SetToBytesDescription(base::checked_cast<size_t>(read_size));
        data_item = new BlobDataItem(std::move(element));
        state = ShareableBlobDataItem::QUOTA_NEEDED;
        break;
      }
      case DataElement::TYPE_FILE: {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.BlobSlice.File",
                                read_size / 1024);
        std::unique_ptr<DataElement> element(new DataElement());
        element->SetToFilePathRange(
            source_item->path(), source_item->offset() + item_offset, read_size,
            source_item->expected_modification_time());
        data_item =
            new BlobDataItem(std::move(element), source_item->data_handle_);

        if (BlobDataBuilder::IsFutureFileItem(source_item->data_element())) {
          if (item_index == first_item_index) {
            first_item_slice_offset = item_offset;
            first_source_item = source_items[item_index];
          } else {
            last_source_item = source_items[item_index];
          }
        }
        break;
      }
      case DataElement::TYPE_FILE_FILESYSTEM: {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.BlobSlice.FileSystem",
                                read_size / 1024);
        std::unique_ptr<DataElement> element(new DataElement());
        element->SetToFileSystemUrlRange(
            source_item->filesystem_url(), source_item->offset() + item_offset,
            read_size, source_item->expected_modification_time());
        data_item = new BlobDataItem(std::move(element));
        break;
      }
      case DataElement::TYPE_DISK_CACHE_ENTRY: {
        UMA_HISTOGRAM_COUNTS_1M("Storage.BlobItemSize.BlobSlice.CacheEntry",
                                read_size / 1024);
        std::unique_ptr<DataElement> element(new DataElement());
        element->SetToDiskCacheEntryRange(source_item->offset() + item_offset,
                                          read_size);
        data_item =
            new BlobDataItem(std::move(element), source_item->data_handle_,
                             source_item->disk_cache_entry(),
                             source_item->disk_cache_stream_index(),
                             source_item->disk_cache_side_stream_index());
        break;
      }
      case DataElement::TYPE_BLOB:
      case DataElement::TYPE_DATA_PIPE:
      case DataElement::TYPE_UNKNOWN:
        CHECK(false) << "Illegal blob item type: " << type;
    }
    dest_items.push_back(
        new ShareableBlobDataItem(std::move(data_item), state));
    item_offset = 0;
  }
}
