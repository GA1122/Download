ScriptPromise Document::createCSSStyleSheet(ScriptState* script_state,
                                            const String& text,
                                            const CSSStyleSheetInit& options,
                                            ExceptionState& exception_state) {
  CSSStyleSheet* sheet = CSSStyleSheet::Create(*this, options, exception_state);
  sheet->SetText(text);
  sheet->SetAssociatedDocument(this);
  return ScriptPromise::Cast(script_state,
                             ScriptValue::From(script_state, sheet));
}
