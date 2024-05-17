int CSSStyleSheet::addRule(const String& selector,
                           const String& style,
                           int index,
                           ExceptionState& exception_state) {
  StringBuilder text;
  text.Append(selector);
  text.Append(" { ");
  text.Append(style);
  if (!style.IsEmpty())
    text.Append(' ');
  text.Append('}');
  insertRule(text.ToString(), index, exception_state);

  return -1;
}
