void RenderBlock::computeSelfHitTestRects(Vector<LayoutRect>& rects, const LayoutPoint& layerOffset) const
{
    RenderBox::computeSelfHitTestRects(rects, layerOffset);

    if (hasHorizontalLayoutOverflow() || hasVerticalLayoutOverflow()) {
        for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox()) {
            LayoutUnit top = max<LayoutUnit>(curr->lineTop(), curr->top());
            LayoutUnit bottom = min<LayoutUnit>(curr->lineBottom(), curr->top() + curr->height());
            LayoutRect rect(layerOffset.x() + curr->x(), layerOffset.y() + top, curr->width(), bottom - top);
            if (!rect.isEmpty() && (rects.isEmpty() || !rects[0].contains(rect)))
                rects.append(rect);
        }
    }
}
