void WebGLRenderingContextBase::SetFilterQuality(
    SkFilterQuality filter_quality) {
  if (!isContextLost() && GetDrawingBuffer()) {
    GetDrawingBuffer()->SetFilterQuality(filter_quality);
  }
}
