void PDFiumEngine::AddSegment(FX_DOWNLOADHINTS* param,
                              size_t offset,
                              size_t size) {
  auto* download_hints = static_cast<DownloadHints*>(param);
  return download_hints->engine->doc_loader_->RequestData(offset, size);
}
