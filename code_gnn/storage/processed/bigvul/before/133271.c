void RenderSVGImage::addFocusRingRects(Vector<IntRect>& rects, const LayoutPoint&, const RenderLayerModelObject*)
{
    IntRect contentRect = enclosingIntRect(repaintRectInLocalCoordinates());
    if (!contentRect.isEmpty())
        rects.append(contentRect);
}
