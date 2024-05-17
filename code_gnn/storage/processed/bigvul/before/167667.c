StyleSheetContents* CSSStyleSheetResource::CreateParsedStyleSheetFromCache(
    const CSSParserContext* context) {
  if (!parsed_style_sheet_cache_)
    return nullptr;
  if (parsed_style_sheet_cache_->HasFailedOrCanceledSubresources()) {
    SetParsedStyleSheetCache(nullptr);
    return nullptr;
  }

  DCHECK(parsed_style_sheet_cache_->IsCacheableForResource());
  DCHECK(parsed_style_sheet_cache_->IsReferencedFromResource());

  if (*parsed_style_sheet_cache_->ParserContext() != *context)
    return nullptr;

  DCHECK(!parsed_style_sheet_cache_->IsLoading());

  if (RuntimeEnabledFeatures::CacheStyleSheetWithMediaQueriesEnabled() &&
      parsed_style_sheet_cache_->HasMediaQueries()) {
    return parsed_style_sheet_cache_->Copy();
  }

  return parsed_style_sheet_cache_;
}
