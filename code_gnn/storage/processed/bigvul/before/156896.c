void DocumentLoader::CommitData(const char* bytes, size_t length) {
  CommitNavigation(response_.MimeType());
  DCHECK_GE(state_, kCommitted);

  if (!frame_ || !frame_->GetDocument()->Parsing())
    return;

  if (length)
    data_received_ = true;

  if (parser_blocked_count_) {
    if (!committed_data_buffer_)
      committed_data_buffer_ = SharedBuffer::Create();
    committed_data_buffer_->Append(bytes, length);
  } else {
    parser_->AppendBytes(bytes, length);
  }
}
