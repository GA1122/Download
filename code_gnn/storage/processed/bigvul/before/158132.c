void LocalFrameClientImpl::UpdateDocumentLoader(
    DocumentLoader* document_loader,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  static_cast<WebDocumentLoaderImpl*>(document_loader)
      ->SetExtraData(std::move(extra_data));
}
