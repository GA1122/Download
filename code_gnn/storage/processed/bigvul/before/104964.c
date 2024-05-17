 void GraphicsContext::fillRect(const FloatRect& rect, const Color& color, ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    savePlatformState();

    m_data->context->SetPen(*wxTRANSPARENT_PEN);
    m_data->context->SetBrush(wxBrush(color));
    m_data->context->DrawRectangle(rect.x(), rect.y(), rect.width(), rect.height());

    restorePlatformState();
}
