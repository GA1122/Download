void RenderBlock::paintContents(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (document().didLayoutWithPendingStylesheets() && !isRenderView())
        return;

    if (childrenInline())
        m_lineBoxes.paint(this, paintInfo, paintOffset);
    else {
        PaintPhase newPhase = (paintInfo.phase == PaintPhaseChildOutlines) ? PaintPhaseOutline : paintInfo.phase;
        newPhase = (newPhase == PaintPhaseChildBlockBackgrounds) ? PaintPhaseChildBlockBackground : newPhase;

        PaintInfo paintInfoForChild(paintInfo);
        paintInfoForChild.phase = newPhase;
        paintInfoForChild.updatePaintingRootForChildren(this);
        paintChildren(paintInfoForChild, paintOffset);
    }
}
