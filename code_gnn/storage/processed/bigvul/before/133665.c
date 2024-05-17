void SVGAnimateElement::resetAnimatedType()
{
    SVGAnimatedTypeAnimator* animator = ensureAnimator();
    ASSERT(m_animatedPropertyType == animator->type());

    SVGElement* targetElement = this->targetElement();
    const QualifiedName& attributeName = this->attributeName();
    ShouldApplyAnimation shouldApply = shouldApplyAnimation(targetElement, attributeName);

    if (shouldApply == DontApplyAnimation)
        return;

    if (shouldApply == ApplyXMLAnimation) {
        m_animatedProperties = animator->findAnimatedPropertiesForAttributeName(targetElement, attributeName);
        SVGElementAnimatedPropertyList::const_iterator end = m_animatedProperties.end();
        for (SVGElementAnimatedPropertyList::const_iterator it = m_animatedProperties.begin(); it != end; ++it)
            document().accessSVGExtensions()->addElementReferencingTarget(this, it->element);

        ASSERT(!m_animatedProperties.isEmpty());

        ASSERT(propertyTypesAreConsistent(m_animatedPropertyType, m_animatedProperties));
        if (!m_animatedType)
            m_animatedType = animator->startAnimValAnimation(m_animatedProperties);
        else {
            animator->resetAnimValToBaseVal(m_animatedProperties, m_animatedType.get());
            animator->animValDidChange(m_animatedProperties);
        }
        return;
    }

    ASSERT(m_animatedProperties.isEmpty());
    String baseValue;

    if (shouldApply == ApplyCSSAnimation) {
        ASSERT(SVGAnimationElement::isTargetAttributeCSSProperty(targetElement, attributeName));
        computeCSSPropertyValue(targetElement, cssPropertyID(attributeName.localName()), baseValue);
    }

    if (!m_animatedType || !m_animatedType->setValueAsString(attributeName, baseValue))
        m_animatedType = animator->constructFromString(baseValue);
}
