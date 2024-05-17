void RenderBlock::addFocusRingRects(Vector<IntRect>& rects, const LayoutPoint& additionalOffset, const RenderLayerModelObject* paintContainer)
{
    if (inlineElementContinuation()) {
        bool nextInlineHasLineBox = inlineElementContinuation()->firstLineBox();
        bool prevInlineHasLineBox = toRenderInline(inlineElementContinuation()->node()->renderer())->firstLineBox();
        LayoutUnit topMargin = prevInlineHasLineBox ? collapsedMarginBefore() : LayoutUnit();
        LayoutUnit bottomMargin = nextInlineHasLineBox ? collapsedMarginAfter() : LayoutUnit();
        LayoutRect rect(additionalOffset.x(), additionalOffset.y() - topMargin, width(), height() + topMargin + bottomMargin);
        if (!rect.isEmpty())
            rects.append(pixelSnappedIntRect(rect));
    } else if (width() && height())
        rects.append(pixelSnappedIntRect(additionalOffset, size()));

    if (!hasOverflowClip() && !hasControlClip()) {
        for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox()) {
            LayoutUnit top = max<LayoutUnit>(curr->lineTop(), curr->top());
            LayoutUnit bottom = min<LayoutUnit>(curr->lineBottom(), curr->top() + curr->height());
            LayoutRect rect(additionalOffset.x() + curr->x(), additionalOffset.y() + top, curr->width(), bottom - top);
            if (!rect.isEmpty())
                rects.append(pixelSnappedIntRect(rect));
        }

        for (RenderObject* curr = firstChild(); curr; curr = curr->nextSibling()) {
            if (!curr->isText() && !curr->isListMarker() && curr->isBox()) {
                RenderBox* box = toRenderBox(curr);
                FloatPoint pos;
                if (box->layer())
                    pos = curr->localToContainerPoint(FloatPoint(), paintContainer);
                else
                    pos = FloatPoint((additionalOffset.x() + box->x()).toFloat(), (additionalOffset.y() + box->y()).toFloat());  
                box->addFocusRingRects(rects, flooredLayoutPoint(pos), paintContainer);
            }
        }
    }

    if (inlineElementContinuation())
        inlineElementContinuation()->addFocusRingRects(rects, flooredLayoutPoint(additionalOffset + inlineElementContinuation()->containingBlock()->location() - location()), paintContainer);
}
