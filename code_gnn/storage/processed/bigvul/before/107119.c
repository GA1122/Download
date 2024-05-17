int64_t LayerTreeHostQt::adoptImageBackingStore(Image* image)
{
    if (!image)
        return InvalidWebLayerID;
    QPixmap* pixmap = image->nativeImageForCurrentFrame();

    if (!pixmap)
        return InvalidWebLayerID;

    int64_t key = pixmap->cacheKey();
    HashMap<int64_t, int>::iterator it = m_directlyCompositedImageRefCounts.find(key);

    if (it != m_directlyCompositedImageRefCounts.end()) {
        ++(it->second);
        return key;
    }

    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::createShareable(image->size(), image->currentFrameHasAlpha() ? ShareableBitmap::SupportsAlpha : 0);
    {
        OwnPtr<WebCore::GraphicsContext> graphicsContext = bitmap->createGraphicsContext();
        graphicsContext->drawImage(image, ColorSpaceDeviceRGB, IntPoint::zero());
    }

    ShareableBitmap::Handle handle;
    bitmap->createHandle(handle);
    m_webPage->send(Messages::LayerTreeHostProxy::CreateDirectlyCompositedImage(key, handle));
    m_directlyCompositedImageRefCounts.add(key, 1);
    return key;
}
