void Document::setupFontBuilder(ComputedStyle& documentStyle)
{
    FontBuilder fontBuilder(*this);
    CSSFontSelector* selector = styleEngine().fontSelector();
    fontBuilder.createFontForDocument(selector, documentStyle);
}
