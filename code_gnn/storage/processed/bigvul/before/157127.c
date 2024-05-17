void ResourceMultiBufferDataProvider::Terminate() {
  fifo_.push_back(DataBuffer::CreateEOSBuffer());
  url_data_->multibuffer()->OnDataProviderEvent(this);
}
