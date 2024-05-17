void RenderBlock::paint(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    ANNOTATE_GRAPHICS_CONTEXT(paintInfo, this);

    LayoutPoint adjustedPaintOffset = paintOffset + location();

    PaintPhase phase = paintInfo.phase;

    LayoutRect overflowBox;
    if (!isDocumentElement()) {
        overflowBox = overflowRectForPaintRejection();
        flipForWritingMode(overflowBox);
        overflowBox.moveBy(adjustedPaintOffset);
        if (!overflowBox.intersects(paintInfo.rect))
            return;
    }

    ContentsClipBehavior contentsClipBehavior = ForceContentsClip;
    if (hasOverflowClip() && !hasControlClip() && !(shouldPaintSelectionGaps() && phase == PaintPhaseForeground) && !hasCaret())
        contentsClipBehavior = SkipContentsClipIfPossible;

    bool pushedClip = pushContentsClip(paintInfo, adjustedPaintOffset, contentsClipBehavior);
    {
        GraphicsContextCullSaver cullSaver(*paintInfo.context);
        bool shouldCull = document().settings()->containerCullingEnabled() && !pushedClip && !isDocumentElement()
            && firstChild() && lastChild() && firstChild() != lastChild();
        if (shouldCull)
            cullSaver.cull(overflowBox);

        paintObject(paintInfo, adjustedPaintOffset);
    }
    if (pushedClip)
        popContentsClip(paintInfo, phase, adjustedPaintOffset);

    if (hasOverflowClip() && style()->visibility() == VISIBLE && (phase == PaintPhaseBlockBackground || phase == PaintPhaseChildBlockBackground) && paintInfo.shouldPaintWithinRoot(this) && !paintInfo.paintRootBackgroundOnly())
        layer()->scrollableArea()->paintOverflowControls(paintInfo.context, roundedIntPoint(adjustedPaintOffset), paintInfo.rect, false  );
}
