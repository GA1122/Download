bool BlobURLRequestJob::ReadItem() {
  if (remaining_bytes_ == 0)
    return true;

  if (current_item_index_ >= blob_data_->items().size()) {
    NotifyFailure(net::ERR_FAILED);
    return false;
  }

  int bytes_to_read = ComputeBytesToRead();

  if (bytes_to_read == 0) {
    AdvanceItem();
    return ReadItem();
  }

  const BlobData::Item& item = blob_data_->items().at(current_item_index_);
  if (item.type() == BlobData::Item::TYPE_BYTES)
    return ReadBytesItem(item, bytes_to_read);
  if (IsFileType(item.type())) {
    return ReadFileItem(GetFileStreamReader(current_item_index_),
                        bytes_to_read);
  }
  NOTREACHED();
  return false;
}
