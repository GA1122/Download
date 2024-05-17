void PDFiumEngine::AddSegment(FX_DOWNLOADHINTS* param,
                              size_t offset, size_t size) {
  PDFiumEngine::DownloadHints* download_hints =
      static_cast<PDFiumEngine::DownloadHints*>(param);
  return download_hints->loader->RequestData(offset, size);
}
