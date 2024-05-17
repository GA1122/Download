bool AppCacheUpdateJob::URLFetcher::ConsumeResponseData(int bytes_read) {
  DCHECK_GT(bytes_read, 0);
  switch (fetch_type_) {
    case MANIFEST_FETCH:
    case MANIFEST_REFETCH:
      manifest_data_.append(buffer_->data(), bytes_read);
      break;
    case URL_FETCH:
    case MASTER_ENTRY_FETCH:
      DCHECK(response_writer_.get());
      response_writer_->WriteData(
          buffer_.get(),
          bytes_read,
          base::Bind(&URLFetcher::OnWriteComplete, base::Unretained(this)));
      return false;   
    default:
      NOTREACHED();
  }
  return true;
}
