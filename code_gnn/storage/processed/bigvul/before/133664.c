void SVGAnimateElement::resetAnimatedPropertyType()
{
    ASSERT(!m_animatedType);
    m_fromType.clear();
    m_toType.clear();
    m_toAtEndOfDurationType.clear();
    m_animator.clear();
    m_animatedPropertyType = targetElement() ? determineAnimatedPropertyType(targetElement()) : AnimatedString;
}
