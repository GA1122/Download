IntRect FrameView::windowClipRectForFrameOwner(const HTMLFrameOwnerElement* ownerElement) const
{
    if (!ownerElement->renderer())
        return windowClipRect();

    const RenderLayer* enclosingLayer = ownerElement->renderer()->enclosingLayer();
    if (!enclosingLayer)
        return windowClipRect();

    DisableCompositingQueryAsserts disabler;

    IntRect clipRect = contentsToWindow(pixelSnappedIntRect(enclosingLayer->clipper().childrenClipRect()));
    return intersection(clipRect, windowClipRect());
}
