bool WebLocalFrameImpl::SetCompositionFromExistingText(
    int composition_start,
    int composition_end,
    const WebVector<WebCompositionUnderline>& underlines) {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::setCompositionFromExistingText");
  if (!GetFrame()->GetEditor().CanEdit())
    return false;

  InputMethodController& input_method_controller =
      GetFrame()->GetInputMethodController();

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  input_method_controller.SetCompositionFromExistingText(
      CompositionUnderlineVectorBuilder::Build(underlines), composition_start,
      composition_end);

  return true;
}
