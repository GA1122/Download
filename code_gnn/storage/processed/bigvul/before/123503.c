void BlobURLRequestJob::AdvanceBytesRead(int result) {
  DCHECK_GT(result, 0);

  current_item_offset_ += result;
  if (current_item_offset_ == item_length_list_[current_item_index_])
    AdvanceItem();

  remaining_bytes_ -= result;
  DCHECK_GE(remaining_bytes_, 0);

  read_buf_->DidConsume(result);
  DCHECK_GE(read_buf_->BytesRemaining(), 0);
}
