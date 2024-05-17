void DocumentLoader::ResumeParser() {
  parser_blocked_count_--;
  DCHECK_GE(parser_blocked_count_, 0);

  if (parser_blocked_count_ != 0)
    return;

  if (committed_data_buffer_ && !committed_data_buffer_->IsEmpty()) {
    base::AutoReset<bool> reentrancy_protector(&in_data_received_, true);

    for (const auto& span : *committed_data_buffer_)
      parser_->AppendBytes(span.data(), span.size());
    committed_data_buffer_->Clear();

    ProcessDataBuffer();
  }

  if (finished_loading_) {
    finished_loading_ = false;
    parser_->Finish();
    parser_.Clear();
  }
}
