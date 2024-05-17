void Instance::DidOpenPreview(int32_t result) {
  if (result == PP_OK) {
    preview_engine_.reset(PDFEngine::Create(new PreviewModeClient(this)));
    preview_engine_->HandleDocumentLoad(embed_preview_loader_);
  } else {
    NOTREACHED();
  }
}
