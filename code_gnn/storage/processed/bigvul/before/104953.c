void GraphicsContext::clipOut(const IntRect& rect)
{
    if (paintingDisabled())
        return;

#if USE(WXGC)
    wxGraphicsContext* gc = m_data->context->GetGraphicsContext();

#ifdef __WXMAC__
    CGContextRef context = (CGContextRef)gc->GetNativeContext();

    CGRect rects[2] = { CGContextGetClipBoundingBox(context), CGRectMake(rect.x(), rect.y(), rect.width(), rect.height()) };
    CGContextBeginPath(context);
    CGContextAddRects(context, rects, 2);
    CGContextEOClip(context);
    return;
#endif

#ifdef __WXMSW__
    Gdiplus::Graphics* g = (Gdiplus::Graphics*)gc->GetNativeContext();
    Gdiplus::Region excludeRegion(Gdiplus::Rect(rect.x(), rect.y(), rect.width(), rect.height()));
    g->ExcludeClip(&excludeRegion);
    return; 
#endif

#endif  

     notImplemented();
 }
