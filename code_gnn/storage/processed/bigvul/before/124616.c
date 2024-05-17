void RenderBlock::paintSelection(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (shouldPaintSelectionGaps() && paintInfo.phase == PaintPhaseForeground) {
        LayoutUnit lastTop = 0;
        LayoutUnit lastLeft = logicalLeftSelectionOffset(this, lastTop);
        LayoutUnit lastRight = logicalRightSelectionOffset(this, lastTop);
        GraphicsContextStateSaver stateSaver(*paintInfo.context);

        LayoutRect gapRectsBounds = selectionGaps(this, paintOffset, LayoutSize(), lastTop, lastLeft, lastRight, &paintInfo);
        if (!gapRectsBounds.isEmpty()) {
            RenderLayer* layer = enclosingLayer();
            gapRectsBounds.moveBy(-paintOffset);
            if (!hasLayer()) {
                LayoutRect localBounds(gapRectsBounds);
                flipForWritingMode(localBounds);
                gapRectsBounds = localToContainerQuad(FloatRect(localBounds), layer->renderer()).enclosingBoundingBox();
                if (layer->renderer()->hasOverflowClip())
                    gapRectsBounds.move(layer->renderBox()->scrolledContentOffset());
            }
            layer->addBlockSelectionGapsBounds(gapRectsBounds);
        }
    }
}
