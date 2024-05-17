void DocumentLoader::ResumeParser() {
  parser_blocked_count_--;
  DCHECK_GE(parser_blocked_count_, 0);

  if (parser_blocked_count_ != 0)
    return;

  if (committed_data_buffer_ && !committed_data_buffer_->IsEmpty()) {
    base::AutoReset<bool> reentrancy_protector(&in_data_received_, true);

    const char* segment;
    size_t pos = 0;
    while (size_t length = committed_data_buffer_->GetSomeData(segment, pos)) {
      parser_->AppendBytes(segment, length);
      pos += length;
    }
    committed_data_buffer_->Clear();

    ProcessDataBuffer();
  }

  if (finished_loading_) {
    finished_loading_ = false;
    parser_->Finish();
    parser_.Clear();
  }
}
