CSSStyleSheet* CSSStyleSheet::Create(StyleSheetContents* sheet,
                                     CSSImportRule* owner_rule) {
  return new CSSStyleSheet(sheet, owner_rule);
}
