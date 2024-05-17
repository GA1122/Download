static bool shouldIgnoreElement(Element* element)
{
    return element->hasTagName(HTMLNames::scriptTag) || element->hasTagName(HTMLNames::noscriptTag) || isCharsetSpecifyingNode(element);
}
