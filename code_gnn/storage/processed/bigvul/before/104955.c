 void GraphicsContext::drawConvexPolygon(size_t npoints, const FloatPoint* points, bool shouldAntialias)
{
    if (paintingDisabled())
        return;

    if (npoints <= 1)
        return;

    wxPoint* polygon = new wxPoint[npoints];
    for (size_t i = 0; i < npoints; i++)
        polygon[i] = wxPoint(points[i].x(), points[i].y());
    m_data->context->SetPen(wxPen(strokeColor(), strokeThickness(), strokeStyleToWxPenStyle(strokeStyle())));
    m_data->context->DrawPolygon((int)npoints, polygon);
    delete [] polygon;
}
