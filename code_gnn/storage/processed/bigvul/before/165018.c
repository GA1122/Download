bool HTMLCanvasElement::IsWebGLBlocked() const {
  Document& document = GetDocument();
  LocalFrame* frame = document.GetFrame();
  return frame && frame->Client()->ShouldBlockWebGL();
}
