Color Document::themeColor() const
{
    for (HTMLMetaElement* metaElement = head() ? Traversal<HTMLMetaElement>::firstChild(*head()) : 0; metaElement; metaElement = Traversal<HTMLMetaElement>::nextSibling(*metaElement)) {
        RGBA32 rgb = Color::transparent;
        if (equalIgnoringCase(metaElement->name(), "theme-color") && CSSParser::parseColor(rgb, metaElement->content().string().stripWhiteSpace(), true))
            return Color(rgb);
    }
    return Color();
}
