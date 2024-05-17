void BlobURLRequestJob::DidReadFile(int result) {
  if (result <= 0) {
    NotifyFailure(net::ERR_FAILED);
    return;
  }
  SetStatus(net::URLRequestStatus());   

  AdvanceBytesRead(result);

  if (!read_buf_->BytesRemaining()) {
    int bytes_read = BytesReadCompleted();
    NotifyReadComplete(bytes_read);
    return;
  }

  int bytes_read = 0;
  if (ReadLoop(&bytes_read))
    NotifyReadComplete(bytes_read);
}
