bool LayoutSVGContainer::hasNonIsolatedBlendingDescendants() const
{
    if (m_hasNonIsolatedBlendingDescendantsDirty) {
        m_hasNonIsolatedBlendingDescendants = SVGLayoutSupport::computeHasNonIsolatedBlendingDescendants(this);
        m_hasNonIsolatedBlendingDescendantsDirty = false;
    }
    return m_hasNonIsolatedBlendingDescendants;
}
