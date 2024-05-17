void WebView::paint(HDC hdc, const IntRect& dirtyRect)
{
    m_page->endPrinting();
    if (useNewDrawingArea()) {
        if (DrawingAreaProxyImpl* drawingArea = static_cast<DrawingAreaProxyImpl*>(m_page->drawingArea())) {
            Region unpaintedRegion;
            drawingArea->paint(hdc, dirtyRect, unpaintedRegion);

            Vector<IntRect> unpaintedRects = unpaintedRegion.rects();
            for (size_t i = 0; i < unpaintedRects.size(); ++i) {
                RECT winRect = unpaintedRects[i];
                drawPageBackground(hdc, unpaintedRects[i]);
            }
        } else
            drawPageBackground(hdc, dirtyRect);

        m_page->didDraw();
    } else {
        if (m_page->isValid() && m_page->drawingArea() && m_page->drawingArea()->paint(dirtyRect, hdc))
            m_page->didDraw();
        else
            drawPageBackground(hdc, dirtyRect);
    }
}
