std::unique_ptr<URLLoaderWrapper> PDFiumEngine::CreateURLLoader() {
  return std::make_unique<URLLoaderWrapperImpl>(GetPluginInstance(),
                                                client_->CreateURLLoader());
}
