void DocumentLoader::CommitData(const char* bytes, size_t length) {
  EnsureWriter(response_.MimeType());
  DCHECK_GE(state_, kCommitted);

  if (!frame_ || !frame_->GetDocument()->Parsing())
    return;

  if (length)
    data_received_ = true;

  writer_->AddData(bytes, length);
}
