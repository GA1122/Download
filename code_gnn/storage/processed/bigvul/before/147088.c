void WebLocalFrameImpl::SetMarkedText(const WebString& text,
                                      unsigned location,
                                      unsigned length) {
  Vector<CompositionUnderline> decorations;
  GetFrame()->GetInputMethodController().SetComposition(text, decorations,
                                                        location, length);
}
