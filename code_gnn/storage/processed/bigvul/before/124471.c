void RenderBlock::absoluteRects(Vector<IntRect>& rects, const LayoutPoint& accumulatedOffset) const
{
    if (isAnonymousBlockContinuation()) {
        rects.append(pixelSnappedIntRect(accumulatedOffset.x(), accumulatedOffset.y() - collapsedMarginBefore(),
                                width(), height() + collapsedMarginBefore() + collapsedMarginAfter()));
        continuation()->absoluteRects(rects, accumulatedOffset - toLayoutSize(location() +
                inlineElementContinuation()->containingBlock()->location()));
    } else
        rects.append(pixelSnappedIntRect(accumulatedOffset, size()));
}
