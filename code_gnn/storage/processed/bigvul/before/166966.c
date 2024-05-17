CSSStyleSheet* CSSStyleSheet::Create(StyleSheetContents* sheet,
                                     Node& owner_node) {
  return new CSSStyleSheet(sheet, owner_node, false,
                           TextPosition::MinimumPosition());
}
