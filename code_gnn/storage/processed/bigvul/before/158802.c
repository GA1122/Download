HTMLImportLoader* HTMLImportsController::LoaderFor(
    const Document& document) const {
  for (const auto& loader : loaders_) {
    if (loader->GetDocument() == &document)
      return loader.Get();
  }

  return nullptr;
}
