CSSStyleSheet* CSSStyleSheet::Create(Document& document,
                                     const String& text,
                                     ExceptionState& exception_state) {
  return CSSStyleSheet::Create(document, text, CSSStyleSheetInit(),
                               exception_state);
}
