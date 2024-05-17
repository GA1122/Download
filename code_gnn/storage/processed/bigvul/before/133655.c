AnimatedPropertyType SVGAnimateElement::determineAnimatedPropertyType(SVGElement* targetElement) const
{
    ASSERT(targetElement);

    Vector<AnimatedPropertyType> propertyTypes;
    targetElement->animatedPropertyTypeForAttribute(attributeName(), propertyTypes);
    if (propertyTypes.isEmpty())
        return AnimatedUnknown;

    ASSERT(propertyTypes.size() <= 2);
    AnimatedPropertyType type = propertyTypes[0];
    if (hasTagName(SVGNames::animateColorTag) && type != AnimatedColor)
        return AnimatedUnknown;

    if (type == AnimatedTransformList && !hasTagName(SVGNames::animateTransformTag))
        return AnimatedUnknown;

    if (targetElement->hasTagName(SVGNames::markerTag) && type == AnimatedAngle) {
        ASSERT(propertyTypes.size() == 2);
        ASSERT(propertyTypes[0] == AnimatedAngle);
        ASSERT(propertyTypes[1] == AnimatedEnumeration);
    } else if (propertyTypes.size() == 2)
        ASSERT(propertyTypes[0] == propertyTypes[1]);

    return type;
}
