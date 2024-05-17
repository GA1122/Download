bool SVGAnimateElement::hasValidAttributeType()
{
    SVGElement* targetElement = this->targetElement();
    if (!targetElement)
        return false;

    return m_animatedPropertyType != AnimatedUnknown && !hasInvalidCSSAttributeType();
}
