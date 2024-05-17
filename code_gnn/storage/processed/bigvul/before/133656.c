SVGAnimatedTypeAnimator* SVGAnimateElement::ensureAnimator()
{
    if (!m_animator)
        m_animator = SVGAnimatorFactory::create(this, targetElement(), m_animatedPropertyType);
    ASSERT(m_animatedPropertyType == m_animator->type());
    return m_animator.get();
}
