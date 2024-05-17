int64_t ResourceMultiBufferDataProvider::block_size() const {
  int64_t ret = 1;
  return ret << url_data_->multibuffer()->block_size_shift();
}
