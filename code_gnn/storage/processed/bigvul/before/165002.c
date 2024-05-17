unsigned HTMLCanvasElement::GetMSAASampleCountFor2dContext() const {
  if (!GetDocument().GetSettings())
    return 0;
  return GetDocument().GetSettings()->GetAccelerated2dCanvasMSAASampleCount();
}
