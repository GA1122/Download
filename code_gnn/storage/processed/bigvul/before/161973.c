PdfMetafileSkia* PrintRenderFrameHelper::PrintPreviewContext::metafile() {
  DCHECK(IsRendering());
  return metafile_.get();
}
