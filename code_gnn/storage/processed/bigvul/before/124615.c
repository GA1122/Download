void RenderBlock::paintObject(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    PaintPhase paintPhase = paintInfo.phase;

    LayoutPoint scrolledOffset = paintOffset;
    if (hasOverflowClip())
        scrolledOffset.move(-scrolledContentOffset());

    if ((paintPhase == PaintPhaseBlockBackground || paintPhase == PaintPhaseChildBlockBackground) && style()->visibility() == VISIBLE) {
        if (hasBoxDecorations())
            paintBoxDecorations(paintInfo, paintOffset);
        if (hasColumns() && !paintInfo.paintRootBackgroundOnly())
            paintColumnRules(paintInfo, scrolledOffset);
    }

    if (paintPhase == PaintPhaseMask && style()->visibility() == VISIBLE) {
        paintMask(paintInfo, paintOffset);
        return;
    }

    if (paintPhase == PaintPhaseClippingMask && style()->visibility() == VISIBLE) {
        paintClippingMask(paintInfo, paintOffset);
        return;
    }

    if (paintPhase == PaintPhaseBlockBackground || paintInfo.paintRootBackgroundOnly())
        return;

    if (paintPhase != PaintPhaseSelfOutline) {
        if (hasColumns())
            paintColumnContents(paintInfo, scrolledOffset);
        else
            paintContents(paintInfo, scrolledOffset);
    }

    bool isPrinting = document().printing();
    if (!isPrinting && !hasColumns())
        paintSelection(paintInfo, scrolledOffset);  

    if (paintPhase == PaintPhaseFloat || paintPhase == PaintPhaseSelection || paintPhase == PaintPhaseTextClip) {
        if (hasColumns())
            paintColumnContents(paintInfo, scrolledOffset, true);
        else
            paintFloats(paintInfo, scrolledOffset, paintPhase == PaintPhaseSelection || paintPhase == PaintPhaseTextClip);
    }

    if ((paintPhase == PaintPhaseOutline || paintPhase == PaintPhaseSelfOutline) && hasOutline() && style()->visibility() == VISIBLE)
        paintOutline(paintInfo, LayoutRect(paintOffset, size()));

    if ((paintPhase == PaintPhaseOutline || paintPhase == PaintPhaseChildOutlines)) {
        RenderInline* inlineCont = inlineElementContinuation();
        if (inlineCont && inlineCont->hasOutline() && inlineCont->style()->visibility() == VISIBLE) {
            RenderInline* inlineRenderer = toRenderInline(inlineCont->node()->renderer());
            RenderBlock* cb = containingBlock();

            bool inlineEnclosedInSelfPaintingLayer = false;
            for (RenderBoxModelObject* box = inlineRenderer; box != cb; box = box->parent()->enclosingBoxModelObject()) {
                if (box->hasSelfPaintingLayer()) {
                    inlineEnclosedInSelfPaintingLayer = true;
                    break;
                }
            }

            if (!inlineEnclosedInSelfPaintingLayer && !hasLayer())
                cb->addContinuationWithOutline(inlineRenderer);
            else if (!inlineRenderer->firstLineBox() || (!inlineEnclosedInSelfPaintingLayer && hasLayer()))
                inlineRenderer->paintOutline(paintInfo, paintOffset - locationOffset() + inlineRenderer->containingBlock()->location());
        }
        paintContinuationOutlines(paintInfo, paintOffset);
    }

    if (paintPhase == PaintPhaseForeground) {
        paintCaret(paintInfo, paintOffset, CursorCaret);
        paintCaret(paintInfo, paintOffset, DragCaret);
    }
}
