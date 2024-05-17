void Document::adjustFloatQuadsForScrollAndAbsoluteZoomAndFrameScale(Vector<FloatQuad>& quads, RenderObject* renderer)
{
    if (!view())
        return;

    float inverseFrameScale = 1;
    if (frame())
        inverseFrameScale = 1 / frame()->frameScaleFactor();

    LayoutRect visibleContentRect = view()->visibleContentRect();
    for (size_t i = 0; i < quads.size(); ++i) {
        quads[i].move(-visibleContentRect.x(), -visibleContentRect.y());
        adjustFloatQuadForAbsoluteZoom(quads[i], renderer);
        if (inverseFrameScale != 1)
            quads[i].scale(inverseFrameScale, inverseFrameScale);
    }
}
