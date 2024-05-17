void DocumentLoader::ProcessDataBuffer() {
  for (const auto& span : *data_buffer_)
    ProcessData(span.data(), span.size());
  data_buffer_->Clear();
}
