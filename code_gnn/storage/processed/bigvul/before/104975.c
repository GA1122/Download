void GraphicsContext::savePlatformState()
{
    if (m_data->context)
    {
#if USE(WXGC)
        wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
        gc->PushState();
#else
    #if __WXMAC__
        CGContextRef context;
        wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
        if (gc)
            context = (CGContextRef)gc->GetNativeContext();
        if (context)
            CGContextSaveGState(context);
    #elif __WXMSW__
        HDC dc = (HDC)m_data->context->GetHDC();
        m_data->mswDCStateID = ::SaveDC(dc);
    #elif __WXGTK__
        m_data->gtkCurrentClipRgn = m_data->context->m_currentClippingRegion;
        m_data->gtkPaintClipRgn = m_data->context->m_paintClippingRegion;
    #endif
#endif  
    }
}
