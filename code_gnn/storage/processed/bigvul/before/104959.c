 void GraphicsContext::drawLineForText(const FloatPoint& origin, float width, bool printing)
{
    if (paintingDisabled())
        return;

    FloatPoint endPoint = origin + FloatSize(width, 0);
    m_data->context->SetPen(wxPen(strokeColor(), strokeThickness(), wxSOLID));
    m_data->context->DrawLine(origin.x(), origin.y(), endPoint.x(), endPoint.y());
}
