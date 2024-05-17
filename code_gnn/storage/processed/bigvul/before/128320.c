bool FrameView::scrollContentsFastPath(const IntSize& scrollDelta)
{
    if (!contentsInCompositedLayer() || hasSlowRepaintObjects())
        return false;

    if (!m_viewportConstrainedObjects || m_viewportConstrainedObjects->isEmpty()) {
        hostWindow()->scroll();
        return true;
    }

    Region regionToUpdate;
    ViewportConstrainedObjectSet::const_iterator end = m_viewportConstrainedObjects->end();
    for (ViewportConstrainedObjectSet::const_iterator it = m_viewportConstrainedObjects->begin(); it != end; ++it) {
        RenderObject* renderer = *it;
        ASSERT(renderer->style()->hasViewportConstrainedPosition());
        ASSERT(renderer->hasLayer());
        RenderLayer* layer = toRenderBoxModelObject(renderer)->layer();

        CompositingState state = layer->compositingState();
        if (state == PaintsIntoOwnBacking || state == PaintsIntoGroupedBacking)
            continue;

        if (layer->subtreeIsInvisible())
            continue;

        if (layer->hasAncestorWithFilterOutsets())
            return false;

        IntRect updateRect = pixelSnappedIntRect(layer->paintInvalidator().paintInvalidationRectIncludingNonCompositingDescendants());

        const RenderLayerModelObject* repaintContainer = layer->renderer()->containerForPaintInvalidation();
        if (repaintContainer && !repaintContainer->isRenderView()) {
            updateRect.moveBy(scrollPosition());
            IntRect previousRect = updateRect;
            previousRect.move(scrollDelta);
            updateRect.unite(previousRect);
            layer->renderer()->invalidatePaintUsingContainer(repaintContainer, updateRect, InvalidationScroll);
        } else {
            updateRect = contentsToRootView(updateRect);
            if (!updateRect.isEmpty())
                regionToUpdate.unite(updateRect);
        }
    }

    hostWindow()->scroll();

    Vector<IntRect> subRectsToUpdate = regionToUpdate.rects();
    size_t viewportConstrainedObjectsCount = subRectsToUpdate.size();
    for (size_t i = 0; i < viewportConstrainedObjectsCount; ++i) {
        IntRect updateRect = subRectsToUpdate[i];
        IntRect scrolledRect = updateRect;
        scrolledRect.move(-scrollDelta);
        updateRect.unite(scrolledRect);
        renderView()->layer()->paintInvalidator().setBackingNeedsPaintInvalidationInRect(rootViewToContents(updateRect));
    }

    return true;
}
