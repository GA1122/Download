bool BlobURLRequestJob::ReadBytesItem(const BlobData::Item& item,
                                      int bytes_to_read) {
  DCHECK_GE(read_buf_->BytesRemaining(), bytes_to_read);

  memcpy(read_buf_->data(),
         item.bytes() + item.offset() + current_item_offset_,
         bytes_to_read);

  AdvanceBytesRead(bytes_to_read);
  return true;
}
