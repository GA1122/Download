float SVGAnimateElement::calculateDistance(const String& fromString, const String& toString)
{
    SVGElement* targetElement = this->targetElement();
    if (!targetElement)
        return -1;

    return ensureAnimator()->calculateDistance(fromString, toString);
}
