void FrameView::updateFixedElementPaintInvalidationRectsAfterScroll()
{
    if (!hasViewportConstrainedObjects())
        return;

    ViewportConstrainedObjectSet::const_iterator end = m_viewportConstrainedObjects->end();
    for (ViewportConstrainedObjectSet::const_iterator it = m_viewportConstrainedObjects->begin(); it != end; ++it) {
        RenderObject* renderer = *it;
        ASSERT(renderer->style()->hasViewportConstrainedPosition());

        ASSERT(renderer->hasLayer());

        RenderLayer* layer = toRenderBoxModelObject(renderer)->layer();

        if (layer->compositingState() == PaintsIntoOwnBacking)
            continue;

        layer->paintInvalidator().computePaintInvalidationRectsIncludingNonCompositingDescendants();
    }
}
