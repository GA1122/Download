static bool isCharsetSpecifyingNode(const Node& node)
{
    if (!isHTMLMetaElement(node))
        return false;

    const HTMLMetaElement& element = toHTMLMetaElement(node);
    HTMLAttributeList attributeList;
    AttributeCollection attributes = element.attributes();
    for (const Attribute& attr: attributes) {
        attributeList.append(std::make_pair(attr.name().localName(), attr.value().string()));
    }
    WTF::TextEncoding textEncoding = encodingFromMetaAttributes(attributeList);
    return textEncoding.isValid();
}
