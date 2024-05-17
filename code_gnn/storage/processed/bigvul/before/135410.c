Color Document::themeColor() const
{
    for (HTMLMetaElement* metaElement = head() ? Traversal<HTMLMetaElement>::firstChild(*head()) : 0; metaElement; metaElement = Traversal<HTMLMetaElement>::nextSibling(*metaElement)) {
        Color color = Color::transparent;
        if (equalIgnoringCase(metaElement->name(), "theme-color") && CSSParser::parseColor(color, metaElement->content().string().stripWhiteSpace(), true))
            return color;
    }
    return Color();
}
