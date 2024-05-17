void GraphicsContext::setPlatformStrokeColor(const Color& color, ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    if (m_data->context)
        m_data->context->SetPen(wxPen(color, strokeThickness(), strokeStyleToWxPenStyle(strokeStyle())));
}
