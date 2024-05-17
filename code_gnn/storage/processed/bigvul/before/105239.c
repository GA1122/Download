void HTMLElement::dirAttributeChanged(const Attribute& attribute)
{
    Element* parent = parentElement();

    if (parent && parent->isHTMLElement() && parent->selfOrAncestorHasDirAutoAttribute())
        toHTMLElement(parent)->adjustDirectionalityIfNeededAfterChildAttributeChanged(this);

    if (equalIgnoringCase(attribute.value(), "auto"))
        calculateAndAdjustDirectionality();
}
