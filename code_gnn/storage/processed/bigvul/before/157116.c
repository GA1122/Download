void ResourceMultiBufferDataProvider::DidFinishLoading() {
  DVLOG(1) << "didFinishLoading";
  DCHECK(active_loader_.get());
  DCHECK(!Available());

  active_loader_.reset();

  int64_t size = byte_pos();

  if (url_data_->length() != kPositionNotSpecified &&
      size < url_data_->length()) {
    if (retries_ < kMaxRetries) {
      DVLOG(1) << " Partial data received.... @ pos = " << size;
      retries_++;
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE,
          base::Bind(&ResourceMultiBufferDataProvider::Start,
                     weak_factory_.GetWeakPtr()),
          base::TimeDelta::FromMilliseconds(kLoaderPartialRetryDelayMs));
      return;
    } else {
      url_data_->Fail();
      return;   
    }
  }

  url_data_->set_length(size);
  fifo_.push_back(DataBuffer::CreateEOSBuffer());

  if (url_data_->url_index()) {
    url_data_->url_index()->TryInsert(url_data_);
  }

  DCHECK(Available());
  url_data_->multibuffer()->OnDataProviderEvent(this);

}
