CSSStyleSheet* Document::createEmptyCSSStyleSheet(
    ScriptState* script_state,
    const CSSStyleSheetInit* options,
    ExceptionState& exception_state) {
  CSSStyleSheet* sheet = CSSStyleSheet::Create(*this, options, exception_state);
  sheet->SetAssociatedDocument(this);
  return sheet;
}
