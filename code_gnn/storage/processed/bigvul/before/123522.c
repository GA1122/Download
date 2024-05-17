void BlobURLRequestJob::Seek(int64 offset) {
  for (current_item_index_ = 0;
       current_item_index_ < blob_data_->items().size() &&
           offset >= item_length_list_[current_item_index_];
       ++current_item_index_) {
    offset -= item_length_list_[current_item_index_];
  }

  current_item_offset_ = offset;

  if (offset == 0)
    return;

  const BlobData::Item& item = blob_data_->items().at(current_item_index_);
  if (IsFileType(item.type())) {
    DeleteCurrentFileReader();
    CreateFileStreamReader(current_item_index_, offset);
  }
}
