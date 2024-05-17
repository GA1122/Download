bool MHTMLPageSerializerDelegate::shouldIgnoreAttribute(const Attribute& attribute)
{
    return attribute.localName() == HTMLNames::srcsetAttr;
}
