void WebPageProxy::setDrawingArea(PassOwnPtr<DrawingAreaProxy> drawingArea)
{
    if (drawingArea == m_drawingArea)
        return;

    m_drawingArea = drawingArea;
}
