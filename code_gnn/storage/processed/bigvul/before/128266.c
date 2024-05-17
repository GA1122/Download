bool FrameView::isEnclosedInCompositingLayer() const
{
    DisableCompositingQueryAsserts disabler;

    RenderObject* frameOwnerRenderer = m_frame->ownerRenderer();
    return frameOwnerRenderer && frameOwnerRenderer->enclosingLayer()->enclosingLayerForPaintInvalidationCrossingFrameBoundaries();
}
