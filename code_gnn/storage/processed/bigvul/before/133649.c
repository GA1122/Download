void SVGAnimateElement::applyResultsToTarget()
{
    ASSERT(m_animatedPropertyType != AnimatedTransformList || hasTagName(SVGNames::animateTransformTag));
    ASSERT(m_animatedPropertyType != AnimatedUnknown);
    ASSERT(m_animator);

    if (!m_animatedType)
        return;

    if (m_animatedProperties.isEmpty()) {
        applyCSSPropertyToTargetAndInstances(targetElement(), attributeName(), m_animatedType->valueAsString());
        return;
    }

    m_animator->animValDidChange(m_animatedProperties);
    notifyTargetAndInstancesAboutAnimValChange(targetElement(), attributeName());
}
