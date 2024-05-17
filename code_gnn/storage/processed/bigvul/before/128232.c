IntRect FrameView::convertFromRenderer(const RenderObject& renderer, const IntRect& rendererRect) const
{
    IntRect rect = pixelSnappedIntRect(enclosingLayoutRect(renderer.localToAbsoluteQuad(FloatRect(rendererRect)).boundingBox()));

    rect.moveBy(-scrollPosition());

    return rect;
}
