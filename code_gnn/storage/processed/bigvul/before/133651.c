bool SVGAnimateElement::calculateFromAndByValues(const String& fromString, const String& byString)
{
    SVGElement* targetElement = this->targetElement();
    if (!targetElement)
        return false;

    if (animationMode() == ByAnimation && !isAdditive())
        return false;

    if (animationMode() == FromByAnimation && !animatedPropertyTypeSupportsAddition())
        return false;

    ASSERT(!hasTagName(SVGNames::setTag));

    determinePropertyValueTypes(fromString, byString);
    ensureAnimator()->calculateFromAndByValues(m_fromType, m_toType, fromString, byString);
    ASSERT(m_animatedPropertyType == m_animator->type());
    return true;
}
