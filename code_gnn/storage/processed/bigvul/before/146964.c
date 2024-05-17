static WebDocumentLoader* DocumentLoaderForDocLoader(DocumentLoader* loader) {
  return loader ? WebDocumentLoaderImpl::FromDocumentLoader(loader) : nullptr;
}
