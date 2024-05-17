void RenderBox::computeRectForRepaint(const RenderLayerModelObject* repaintContainer, LayoutRect& rect, bool fixed) const
{
    RenderStyle* styleToUse = style();
    if (RenderView* v = view()) {
        if (v->canUseLayoutStateForContainer(repaintContainer) && styleToUse->position() != FixedPosition) {
            LayoutState* layoutState = v->layoutState();

            if (layer() && layer()->transform())
                rect = layer()->transform()->mapRect(pixelSnappedIntRect(rect));

            if (styleToUse->hasInFlowPosition() && layer())
                rect.move(layer()->offsetForInFlowPosition());

            rect.moveBy(location());
            rect.move(layoutState->paintOffset());
            if (layoutState->isClipped())
                rect.intersect(layoutState->clipRect());
            return;
        }
    }

    if (hasReflection())
        rect.unite(reflectedRect(rect));

    if (repaintContainer == this) {
        if (repaintContainer->style()->isFlippedBlocksWritingMode())
            flipForWritingMode(rect);
        return;
    }

    bool containerSkipped;
    RenderObject* o = container(repaintContainer, &containerSkipped);
    if (!o)
        return;

    if (isWritingModeRoot() && !isOutOfFlowPositioned())
        flipForWritingMode(rect);

    LayoutPoint topLeft = rect.location();
    topLeft.move(locationOffset());

    EPosition position = styleToUse->position();

    if (hasLayer() && layer()->transform()) {
        fixed = position == FixedPosition;
        rect = layer()->transform()->mapRect(pixelSnappedIntRect(rect));
        topLeft = rect.location();
        topLeft.move(locationOffset());
    } else if (position == FixedPosition)
        fixed = true;

    if (position == AbsolutePosition && o->isInFlowPositioned() && o->isRenderInline()) {
        topLeft += toRenderInline(o)->offsetForInFlowPositionedInline(*this);
    } else if (styleToUse->hasInFlowPosition() && layer()) {
        topLeft += layer()->offsetForInFlowPosition();
    }

    if (position != AbsolutePosition && position != FixedPosition && o->hasColumns() && o->isRenderBlockFlow()) {
        LayoutRect repaintRect(topLeft, rect.size());
        toRenderBlock(o)->adjustRectForColumns(repaintRect);
        topLeft = repaintRect.location();
        rect = repaintRect;
    }

    rect.setLocation(topLeft);
    if (o->hasOverflowClip()) {
        RenderBox* containerBox = toRenderBox(o);
        containerBox->applyCachedClipAndScrollOffsetForRepaint(rect);
        if (rect.isEmpty())
            return;
    }

    if (containerSkipped) {
        LayoutSize containerOffset = repaintContainer->offsetFromAncestorContainer(o);
        rect.move(-containerOffset);
        return;
    }

    o->computeRectForRepaint(repaintContainer, rect, fixed);
}
