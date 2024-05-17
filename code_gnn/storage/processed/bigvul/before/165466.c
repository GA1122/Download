ScriptPromise Document::createCSSStyleSheet(ScriptState* script_state,
                                            const String& text,
                                            ExceptionState& exception_state) {
  return Document::createCSSStyleSheet(
      script_state, text, CSSStyleSheetInit::Create(), exception_state);
}
