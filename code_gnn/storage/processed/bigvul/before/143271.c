void Document::setShadowCascadeOrder(ShadowCascadeOrder order)
{
    DCHECK_NE(order, ShadowCascadeOrder::ShadowCascadeNone);

    if (order == m_shadowCascadeOrder)
        return;

    if (order == ShadowCascadeOrder::ShadowCascadeV0) {
        m_mayContainV0Shadow = true;
        if (m_shadowCascadeOrder == ShadowCascadeOrder::ShadowCascadeV1)
            UseCounter::count(*this, UseCounter::MixedShadowRootV0AndV1);
    }

    if (m_shadowCascadeOrder == ShadowCascadeOrder::ShadowCascadeV0 && order == ShadowCascadeOrder::ShadowCascadeV1) {
        this->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::Shadow));
        UseCounter::count(*this, UseCounter::MixedShadowRootV0AndV1);
    }

    if (order > m_shadowCascadeOrder)
        m_shadowCascadeOrder = order;
}
