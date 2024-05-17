PassRefPtr<NamedNodeMap> cloneAttributes(Element* element)
{
    NamedNodeMap* attributes = element->attributes(true);
    if (!attributes)
        return 0;

    RefPtr<NamedNodeMap> newAttributes = NamedNodeMap::create();
    for (size_t i = 0; i < attributes->length(); ++i) {
        Attribute* attribute = attributes->attributeItem(i);
        RefPtr<Attribute> clone = Attribute::createMapped(attribute->name(), attribute->value());
        newAttributes->addAttribute(clone);
    }
    return newAttributes.release();
}
