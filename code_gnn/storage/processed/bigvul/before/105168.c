static bool allowsAuthorShadowRoot(Element* element)
{
    if (element->hasTagName(HTMLNames::videoTag) || element->hasTagName(HTMLNames::audioTag))
        return false;

    if (element->isFormControlElement())
        return false;

    if (element->isSVGElement())
        return false;

    return true;
}
