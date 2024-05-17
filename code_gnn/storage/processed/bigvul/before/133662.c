static inline void removeCSSPropertyFromTarget(SVGElement* targetElement, CSSPropertyID id)
{
    ASSERT_WITH_SECURITY_IMPLICATION(!targetElement->m_deletionHasBegun);
    targetElement->ensureAnimatedSMILStyleProperties()->removeProperty(id);
    targetElement->setNeedsStyleRecalc(LocalStyleChange);
}
