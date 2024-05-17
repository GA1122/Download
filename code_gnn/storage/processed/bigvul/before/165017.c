bool HTMLCanvasElement::IsWebGL2Enabled() const {
  Document& document = GetDocument();
  LocalFrame* frame = document.GetFrame();
  if (!frame)
    return false;
  Settings* settings = frame->GetSettings();
  return settings && settings->GetWebGL2Enabled();
}
