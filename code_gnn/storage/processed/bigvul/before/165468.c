CSSStyleSheet* Document::createCSSStyleSheetSync(
    ScriptState* script_state,
    const String& text,
    const CSSStyleSheetInit* options,
    ExceptionState& exception_state) {
  CSSStyleSheet* sheet = CSSStyleSheet::Create(*this, options, exception_state);
  sheet->SetText(text, false  , exception_state);
  if (exception_state.HadException())
    return nullptr;
  sheet->SetAssociatedDocument(this);
  return sheet;
}
