void GraphicsContext::releaseWindowsContext(HDC hdc, const IntRect& dstRect, bool supportAlphaBlend, bool mayCreateBitmap)
{
    if (hdc) {

        if (!dstRect.isEmpty()) {

            HBITMAP bitmap = static_cast<HBITMAP>(GetCurrentObject(hdc, OBJ_BITMAP));
            BITMAP info;
            GetObject(bitmap, sizeof(info), &info);
            ASSERT(info.bmBitsPixel == 32);

            wxBitmap bmp;
            bmp.SetHBITMAP(bitmap);
#if !wxCHECK_VERSION(2,9,0)
            if (supportAlphaBlend)
                bmp.UseAlpha();
#endif
            m_data->context->DrawBitmap(bmp, dstRect.x(), dstRect.y(), supportAlphaBlend);

            ::DeleteObject(bitmap);
        }

        ::DeleteDC(hdc);
    }
}
