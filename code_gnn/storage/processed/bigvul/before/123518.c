bool BlobURLRequestJob::ReadFileItem(FileStreamReader* reader,
                                     int bytes_to_read) {
  DCHECK_GE(read_buf_->BytesRemaining(), bytes_to_read);
  DCHECK(reader);
  const int result = reader->Read(
      read_buf_, bytes_to_read,
      base::Bind(&BlobURLRequestJob::DidReadFile,
                 base::Unretained(this)));
  if (result >= 0) {
    if (GetStatus().is_io_pending())
      DidReadFile(result);
    else
      AdvanceBytesRead(result);
    return true;
  }
  if (result == net::ERR_IO_PENDING)
    SetStatus(net::URLRequestStatus(net::URLRequestStatus::IO_PENDING, 0));
  else
    NotifyFailure(result);
  return false;
}
