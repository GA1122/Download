bool PrintWebViewHelper::PrintPreviewContext::IsModifiable() const {
  return !PrintingNodeOrPdfFrame(frame_, node_);
}
