bool SVGAnimateElement::animatedPropertyTypeSupportsAddition() const
{
    switch (m_animatedPropertyType) {
    case AnimatedBoolean:
    case AnimatedEnumeration:
    case AnimatedPreserveAspectRatio:
    case AnimatedString:
    case AnimatedUnknown:
        return false;
    default:
        return true;
    }
}
