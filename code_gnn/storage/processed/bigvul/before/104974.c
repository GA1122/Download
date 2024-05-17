FloatRect GraphicsContext::roundToDevicePixels(const FloatRect& frect, RoundingMode)
{
    FloatRect result;

    wxCoord x = (wxCoord)frect.x();
    wxCoord y = (wxCoord)frect.y();

    x = m_data->context->LogicalToDeviceX(x);
    y = m_data->context->LogicalToDeviceY(y);
    result.setX((float)x);
    result.setY((float)y);
    x = (wxCoord)frect.width();
    y = (wxCoord)frect.height();
    x = m_data->context->LogicalToDeviceXRel(x);
    y = m_data->context->LogicalToDeviceYRel(y);
    result.setWidth((float)x);
    result.setHeight((float)y);
    return result; 
}
