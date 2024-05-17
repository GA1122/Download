bool RenderBox::pushContentsClip(PaintInfo& paintInfo, const LayoutPoint& accumulatedOffset, ContentsClipBehavior contentsClipBehavior)
{
    if (paintInfo.phase == PaintPhaseBlockBackground || paintInfo.phase == PaintPhaseSelfOutline || paintInfo.phase == PaintPhaseMask)
        return false;

    bool isControlClip = hasControlClip();
    bool isOverflowClip = hasOverflowClip() && !layer()->isSelfPaintingLayer();

    if (!isControlClip && !isOverflowClip)
        return false;

    LayoutRect clipRect = isControlClip ? controlClipRect(accumulatedOffset) : overflowClipRect(accumulatedOffset);
    RoundedRect clipRoundedRect(0, 0, 0, 0);
    bool hasBorderRadius = style()->hasBorderRadius();
    if (hasBorderRadius)
        clipRoundedRect = style()->getRoundedInnerBorderFor(LayoutRect(accumulatedOffset, size()));

    if (contentsClipBehavior == SkipContentsClipIfPossible) {
        LayoutRect contentsVisualOverflow = contentsVisualOverflowRect();
        if (contentsVisualOverflow.isEmpty())
            return false;

        LayoutRect conservativeClipRect = clipRect;
        if (hasBorderRadius)
            conservativeClipRect.intersect(clipRoundedRect.radiusCenterRect());
        conservativeClipRect.moveBy(-accumulatedOffset);
        if (hasLayer())
            conservativeClipRect.move(scrolledContentOffset());
        if (conservativeClipRect.contains(contentsVisualOverflow))
            return false;
    }

    if (paintInfo.phase == PaintPhaseOutline)
        paintInfo.phase = PaintPhaseChildOutlines;
    else if (paintInfo.phase == PaintPhaseChildBlockBackground) {
        paintInfo.phase = PaintPhaseBlockBackground;
        paintObject(paintInfo, accumulatedOffset);
        paintInfo.phase = PaintPhaseChildBlockBackgrounds;
    }
    paintInfo.context->save();
    if (hasBorderRadius)
        paintInfo.context->clipRoundedRect(clipRoundedRect);
    paintInfo.context->clip(pixelSnappedIntRect(clipRect));
    return true;
}
