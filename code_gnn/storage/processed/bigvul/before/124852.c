void RenderBox::computeSelfHitTestRects(Vector<LayoutRect>& rects, const LayoutPoint& layerOffset) const
{
    if (!size().isEmpty())
        rects.append(LayoutRect(layerOffset, size()));
}
