void LayoutSVGContainer::descendantIsolationRequirementsChanged(DescendantIsolationState state)
{
    switch (state) {
    case DescendantIsolationRequired:
        m_hasNonIsolatedBlendingDescendants = true;
        m_hasNonIsolatedBlendingDescendantsDirty = false;
        break;
    case DescendantIsolationNeedsUpdate:
        if (m_hasNonIsolatedBlendingDescendantsDirty)
            return;
        m_hasNonIsolatedBlendingDescendantsDirty = true;
        break;
    }
    if (SVGLayoutSupport::willIsolateBlendingDescendantsForObject(this))
        return;
    if (parent())
        parent()->descendantIsolationRequirementsChanged(state);
}
