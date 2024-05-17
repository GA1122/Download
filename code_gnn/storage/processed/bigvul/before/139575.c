static String ValueStyle(LocalFrame& frame, CSSPropertyID property_id) {
  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  return frame.GetEditor().SelectionStartCSSPropertyValue(property_id);
}
