void CSSStyleSheetResource::SaveParsedStyleSheet(StyleSheetContents* sheet) {
  DCHECK(sheet);
  DCHECK(sheet->IsCacheableForResource());

  if (!GetMemoryCache()->Contains(this)) {
    SetParsedStyleSheetCache(nullptr);
    return;
  }
  SetParsedStyleSheetCache(sheet);
}
