static bool findAttributeWithName(const HTMLToken& token, const QualifiedName& name, size_t& indexOfMatchingAttribute)
{
    const String& attrName = name.namespaceURI() == XLinkNames::xlinkNamespaceURI ? "xlink:" + name.localName().string() : name.localName().string();

    for (size_t i = 0; i < token.attributes().size(); ++i) {
        if (equalIgnoringNullity(token.attributes().at(i).name, attrName)) {
            indexOfMatchingAttribute = i;
            return true;
        }
    }
    return false;
}
