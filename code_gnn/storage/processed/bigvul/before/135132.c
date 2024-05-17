void Document::adjustFloatQuadsForScrollAndAbsoluteZoom(Vector<FloatQuad>& quads, LayoutObject& layoutObject)
{
    if (!view())
        return;

    LayoutRect visibleContentRect(view()->visibleContentRect());
    for (size_t i = 0; i < quads.size(); ++i) {
        quads[i].move(-FloatSize(visibleContentRect.x().toFloat(), visibleContentRect.y().toFloat()));
        adjustFloatQuadForAbsoluteZoom(quads[i], layoutObject);
    }
}
