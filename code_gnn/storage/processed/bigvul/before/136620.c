const AtomicString& DocumentLoader::MimeType() const {
  if (fetcher_->Archive())
    return fetcher_->Archive()->MainResource()->MimeType();
  return response_.MimeType();
}
