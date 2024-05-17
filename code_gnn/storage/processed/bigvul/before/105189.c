void StyledMarkupAccumulator::appendStyleNodeOpenTag(StringBuilder& out, StylePropertySet* style, Document* document, bool isBlock)
{
    ASSERT(propertyMissingOrEqualToNone(style, CSSPropertyWebkitTextDecorationsInEffect));
    DEFINE_STATIC_LOCAL(const String, divStyle, ("<div style=\""));
    DEFINE_STATIC_LOCAL(const String, styleSpanOpen, ("<span style=\""));
    out.append(isBlock ? divStyle : styleSpanOpen);
    appendAttributeValue(out, style->asText(), document->isHTMLDocument());
    out.append('\"');
    out.append('>');
}
