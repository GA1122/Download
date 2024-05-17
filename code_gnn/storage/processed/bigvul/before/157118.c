void ResourceMultiBufferDataProvider::DidReceiveData(const char* data,
                                                     int data_length) {
  DVLOG(1) << "didReceiveData: " << data_length << " bytes";
  DCHECK(!Available());
  DCHECK(active_loader_);
  DCHECK_GT(data_length, 0);

  url_data_->AddBytesReadFromNetwork(data_length);

  if (bytes_to_discard_) {
    uint64_t tmp = std::min<uint64_t>(bytes_to_discard_, data_length);
    data_length -= tmp;
    data += tmp;
    bytes_to_discard_ -= tmp;
    if (data_length == 0)
      return;
  }

  retries_ = 0;

  while (data_length) {
    if (fifo_.empty() || fifo_.back()->data_size() == block_size()) {
      fifo_.push_back(new DataBuffer(block_size()));
      fifo_.back()->set_data_size(0);
    }
    int last_block_size = fifo_.back()->data_size();
    int to_append = std::min<int>(data_length, block_size() - last_block_size);
    DCHECK_GT(to_append, 0);
    memcpy(fifo_.back()->writable_data() + last_block_size, data, to_append);
    data += to_append;
    fifo_.back()->set_data_size(last_block_size + to_append);
    data_length -= to_append;
  }

  url_data_->multibuffer()->OnDataProviderEvent(this);

}
