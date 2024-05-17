const Document* CSSStyleSheet::SingleOwnerDocument(
    const CSSStyleSheet* style_sheet) {
  if (style_sheet)
    return StyleSheetContents::SingleOwnerDocument(style_sheet->Contents());
  return nullptr;
}
