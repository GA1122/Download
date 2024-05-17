void WebView::setFindIndicator(PassRefPtr<FindIndicator> prpFindIndicator, bool fadeOut)
{
    if (!m_findIndicatorCallback)
        return;

    HBITMAP hbmp = 0;
    IntRect selectionRect;

    if (RefPtr<FindIndicator> findIndicator = prpFindIndicator) {
        if (ShareableBitmap* contentImage = findIndicator->contentImage()) {
            void* bits;
            HDC hdc = ::CreateCompatibleDC(0);
            int width = contentImage->bounds().width();
            int height = contentImage->bounds().height();
            BitmapInfo bitmapInfo = BitmapInfo::create(contentImage->size());

            hbmp = CreateDIBSection(0, &bitmapInfo, DIB_RGB_COLORS, static_cast<void**>(&bits), 0, 0);
            HBITMAP hbmpOld = static_cast<HBITMAP>(SelectObject(hdc, hbmp));
#if PLATFORM(CG)
            RetainPtr<CGContextRef> context(AdoptCF, CGBitmapContextCreate(bits, width, height,
                8, width * sizeof(RGBQUAD), deviceRGBColorSpaceRef(), kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst));

            GraphicsContext graphicsContext(context.get());
            contentImage->paint(graphicsContext, IntPoint(), contentImage->bounds());
#else
#endif

            ::SelectObject(hdc, hbmpOld);
            ::DeleteDC(hdc);
        }

        selectionRect = IntRect(findIndicator->selectionRectInWindowCoordinates());
    }
    
    (*m_findIndicatorCallback)(toAPI(this), hbmp, selectionRect, fadeOut, m_findIndicatorCallbackContext);
}
