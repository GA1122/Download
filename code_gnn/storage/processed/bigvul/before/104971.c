void GraphicsContext::platformInit(PlatformGraphicsContext* context)
{
    m_data = new GraphicsContextPlatformPrivate;
    setPaintingDisabled(!context);

    if (context) {
        setPlatformFillColor(fillColor(), ColorSpaceDeviceRGB);
        setPlatformStrokeColor(strokeColor(), ColorSpaceDeviceRGB);
    }
#if USE(WXGC)
    m_data->context = (wxGCDC*)context;
#else
    m_data->context = (wxWindowDC*)context;
#endif
}
