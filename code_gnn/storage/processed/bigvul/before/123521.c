bool BlobURLRequestJob::ReadRawData(net::IOBuffer* dest,
                                    int dest_size,
                                    int* bytes_read) {
  DCHECK_NE(dest_size, 0);
  DCHECK(bytes_read);
  DCHECK_GE(remaining_bytes_, 0);

  if (error_) {
    *bytes_read = 0;
    return true;
  }

  if (remaining_bytes_ < dest_size)
    dest_size = static_cast<int>(remaining_bytes_);

  if (!dest_size) {
    *bytes_read = 0;
    return true;
  }

  DCHECK(!read_buf_);
  read_buf_ = new net::DrainableIOBuffer(dest, dest_size);

  return ReadLoop(bytes_read);
}
