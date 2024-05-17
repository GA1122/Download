 bool BlobURLRequestJob::ReadLoop(int* bytes_read) {
  while (remaining_bytes_ > 0 && read_buf_->BytesRemaining() > 0) {
    if (!ReadItem())
      return false;
  }

  *bytes_read = BytesReadCompleted();
  return true;
}
