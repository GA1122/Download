void CSSStyleSheetResource::DestroyDecodedDataIfPossible() {
  if (!parsed_style_sheet_cache_)
    return;

  SetParsedStyleSheetCache(nullptr);
}
