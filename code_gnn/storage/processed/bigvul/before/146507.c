void WebGLRenderingContextBase::getHTMLOrOffscreenCanvas(
    HTMLCanvasElementOrOffscreenCanvas& result) const {
  if (canvas()) {
    result.setHTMLCanvasElement(static_cast<HTMLCanvasElement*>(host()));
  } else {
    result.setOffscreenCanvas(static_cast<OffscreenCanvas*>(host()));
  }
}
