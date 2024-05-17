void SVGAnimateElement::clearAnimatedType(SVGElement* targetElement)
{
    if (!m_animatedType)
        return;

    if (!targetElement) {
        m_animatedType.clear();
        return;
    }

    if (m_animatedProperties.isEmpty()) {
        removeCSSPropertyFromTargetAndInstances(targetElement, attributeName());
        m_animatedType.clear();
        return;
    }

    if (m_animator) {
        m_animator->stopAnimValAnimation(m_animatedProperties);
        notifyTargetAndInstancesAboutAnimValChange(targetElement, attributeName());
    }

    m_animatedProperties.clear();
    m_animatedType.clear();
}
