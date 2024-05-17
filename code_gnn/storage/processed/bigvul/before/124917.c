void RenderBox::paintClippingMask(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (!paintInfo.shouldPaintWithinRoot(this) || style()->visibility() != VISIBLE || paintInfo.phase != PaintPhaseClippingMask || paintInfo.context->paintingDisabled())
        return;

    if (!layer() || layer()->compositingState() != PaintsIntoOwnBacking)
        return;

    ASSERT(layer()->compositingState() != HasOwnBackingButPaintsIntoAncestor);

    LayoutRect paintRect = LayoutRect(paintOffset, size());
    paintInfo.context->fillRect(pixelSnappedIntRect(paintRect), Color::black);
}
