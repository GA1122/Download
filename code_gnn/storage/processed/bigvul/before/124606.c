void RenderBlock::paintAsInlineBlock(RenderObject* renderer, PaintInfo& paintInfo, const LayoutPoint& childPoint)
{
    if (paintInfo.phase != PaintPhaseForeground && paintInfo.phase != PaintPhaseSelection)
        return;

    bool preservePhase = paintInfo.phase == PaintPhaseSelection || paintInfo.phase == PaintPhaseTextClip;
    PaintInfo info(paintInfo);
    info.phase = preservePhase ? paintInfo.phase : PaintPhaseBlockBackground;
    renderer->paint(info, childPoint);
    if (!preservePhase) {
        info.phase = PaintPhaseChildBlockBackgrounds;
        renderer->paint(info, childPoint);
        info.phase = PaintPhaseFloat;
        renderer->paint(info, childPoint);
        info.phase = PaintPhaseForeground;
        renderer->paint(info, childPoint);
        info.phase = PaintPhaseOutline;
        renderer->paint(info, childPoint);
    }
}
