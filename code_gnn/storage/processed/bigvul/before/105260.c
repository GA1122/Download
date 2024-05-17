static unsigned parseBorderWidthAttribute(const Attribute& attribute)
{
    ASSERT(attribute.name() == borderAttr);
    unsigned borderWidth = 0;
    if (!attribute.isEmpty())
        parseHTMLNonNegativeInteger(attribute.value(), borderWidth);
    return borderWidth;
}
