void Document::setupFontBuilder(ComputedStyle& documentStyle)
{
    FontBuilder fontBuilder(*this);
    RefPtrWillBeRawPtr<CSSFontSelector> selector = styleEngine().fontSelector();
    fontBuilder.createFontForDocument(selector, documentStyle);
}
