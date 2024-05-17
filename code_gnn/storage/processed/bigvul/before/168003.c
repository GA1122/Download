void DocumentLoader::ProcessDataBuffer() {
  const char* segment;
  size_t pos = 0;
  while (size_t length = data_buffer_->GetSomeData(segment, pos)) {
    ProcessData(segment, length);
    pos += length;
  }
  data_buffer_->Clear();
}
