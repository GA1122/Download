unsigned long DocumentLoader::MainResourceIdentifier() const {
  return GetResource() ? GetResource()->Identifier() : 0;
}
