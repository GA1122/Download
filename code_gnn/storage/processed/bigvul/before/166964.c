CSSStyleSheet* CSSStyleSheet::Create(Document& document,
                                     const String& text,
                                     const CSSStyleSheetInit& options,
                                     ExceptionState& exception_state) {
  if (!RuntimeEnabledFeatures::ConstructableStylesheetsEnabled()) {
    exception_state.ThrowTypeError("Illegal constructor");
    return nullptr;
  }
  CSSParserContext* parser_context = CSSParserContext::Create(document);
  StyleSheetContents* contents = StyleSheetContents::Create(parser_context);
  CSSStyleSheet* sheet = new CSSStyleSheet(contents, nullptr);
  sheet->SetTitle(options.title());
  sheet->ClearOwnerNode();
  sheet->ClearOwnerRule();
  if (options.media().IsString()) {
    MediaList* media_list = MediaList::Create(
        MediaQuerySet::Create(), const_cast<CSSStyleSheet*>(sheet));
    media_list->setMediaText(options.media().GetAsString());
    sheet->SetMedia(media_list);
  } else {
    sheet->SetMedia(options.media().GetAsMediaList());
  }
  if (options.alternate())
    sheet->SetAlternateFromConstructor(true);
  if (options.disabled())
    sheet->setDisabled(true);
  sheet->SetText(text);
  return sheet;
}
