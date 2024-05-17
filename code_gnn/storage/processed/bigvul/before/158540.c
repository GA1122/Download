bool WebLocalFrameImpl::FirstRectForCharacterRange(
    unsigned location,
    unsigned length,
    WebRect& rect_in_viewport) const {
  if ((location + length < location) && (location + length))
    length = 0;

  Element* editable =
      GetFrame()->Selection().RootEditableElementOrDocumentElement();
  if (!editable)
    return false;

  editable->GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  const EphemeralRange range =
      PlainTextRange(location, location + length).CreateRange(*editable);
  if (range.IsNull())
    return false;
  IntRect int_rect = FirstRectForRange(range);
  rect_in_viewport = WebRect(int_rect);
  rect_in_viewport = GetFrame()->View()->FrameToViewport(rect_in_viewport);
  return true;
}
