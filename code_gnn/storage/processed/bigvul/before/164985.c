LayoutObject* HTMLCanvasElement::CreateLayoutObject(
    const ComputedStyle& style) {
  LocalFrame* frame = GetDocument().GetFrame();
  if (frame && GetDocument().CanExecuteScripts(kNotAboutToExecuteScript))
    return new LayoutHTMLCanvas(this);
  return HTMLElement::CreateLayoutObject(style);
}
