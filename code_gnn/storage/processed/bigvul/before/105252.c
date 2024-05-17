bool HTMLElement::isURLAttribute(const Attribute& attribute) const
{
#if ENABLE(MICRODATA)
    return attribute.name() == itemidAttr;
#else
    UNUSED_PARAM(attribute);
    return false;
#endif
}
