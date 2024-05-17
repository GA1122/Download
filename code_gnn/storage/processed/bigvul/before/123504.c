void BlobURLRequestJob::AdvanceItem() {
  DeleteCurrentFileReader();

  current_item_index_++;
  current_item_offset_ = 0;
}
