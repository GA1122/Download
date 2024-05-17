const AtomicString& DocumentLoader::MimeType() const {
  if (writer_)
    return writer_->MimeType();
  return response_.MimeType();
}
