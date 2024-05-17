const KURL& DocumentLoader::UnreachableURL() const {
  return substitute_data_.FailingURL();
}
