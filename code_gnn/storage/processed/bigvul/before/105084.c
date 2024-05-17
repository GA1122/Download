static void adjustFloatQuadsForScrollAndAbsoluteZoomAndPageScale(Vector<FloatQuad>& quads, Document* document, RenderObject* renderer)
{
    FrameView* view = document->view();
    if (!view)
        return;

    float pageScale = 1;
    if (Page* page = document->page())
        pageScale = page->pageScaleFactor();

    LayoutRect visibleContentRect = view->visibleContentRect();
    for (size_t i = 0; i < quads.size(); ++i) {
        quads[i].move(-visibleContentRect.x(), -visibleContentRect.y());
        adjustFloatQuadForAbsoluteZoom(quads[i], renderer);
        if (pageScale != 1)
            adjustFloatQuadForPageScale(quads[i], pageScale);
    }
}
