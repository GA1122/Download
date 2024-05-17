void CSSStyleSheetResource::Trace(blink::Visitor* visitor) {
  visitor->Trace(parsed_style_sheet_cache_);
  TextResource::Trace(visitor);
}
