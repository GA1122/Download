CSSStyleSheet* Document::createCSSStyleSheetSync(
    ScriptState* script_state,
    const String& text,
    ExceptionState& exception_state) {
  return Document::createCSSStyleSheetSync(
      script_state, text, CSSStyleSheetInit::Create(), exception_state);
}
