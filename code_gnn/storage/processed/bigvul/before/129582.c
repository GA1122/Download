void LayoutSVGContainer::updateCachedBoundaries()
{
    SVGLayoutSupport::computeContainerBoundingBoxes(this, m_objectBoundingBox, m_objectBoundingBoxValid, m_strokeBoundingBox, m_paintInvalidationBoundingBox);
    SVGLayoutSupport::intersectPaintInvalidationRectWithResources(this, m_paintInvalidationBoundingBox);
}
