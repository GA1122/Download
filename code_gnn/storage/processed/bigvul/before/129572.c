LayoutSVGContainer::LayoutSVGContainer(SVGElement* node)
    : LayoutSVGModelObject(node)
    , m_objectBoundingBoxValid(false)
    , m_needsBoundariesUpdate(true)
    , m_didScreenScaleFactorChange(false)
    , m_hasNonIsolatedBlendingDescendants(false)
    , m_hasNonIsolatedBlendingDescendantsDirty(false)
{
}
