void HTMLInputElement::setType(const String& type)
{
    if (type.isEmpty())
        removeAttribute(typeAttr);
    else
        setAttribute(typeAttr, type);
}
