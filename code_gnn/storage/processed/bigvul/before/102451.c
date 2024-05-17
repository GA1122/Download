int64_t LayerTreeCoordinator::adoptImageBackingStore(Image* image)
{
    if (!image)
        return InvalidWebLayerID;

    int64_t key = 0;

#if PLATFORM(QT)
    QImage* nativeImage = image->nativeImageForCurrentFrame();

    if (!nativeImage)
        return InvalidWebLayerID;

    key = nativeImage->cacheKey();
#endif

    HashMap<int64_t, int>::iterator it = m_directlyCompositedImageRefCounts.find(key);

    if (it != m_directlyCompositedImageRefCounts.end()) {
        ++(it->second);
        return key;
    }

    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::createShareable(image->size(), (image->currentFrameHasAlpha() ? ShareableBitmap::SupportsAlpha : 0));
    {
        OwnPtr<WebCore::GraphicsContext> graphicsContext = bitmap->createGraphicsContext();
        graphicsContext->drawImage(image, ColorSpaceDeviceRGB, IntPoint::zero());
    }

    ShareableBitmap::Handle handle;
    bitmap->createHandle(handle);
    m_webPage->send(Messages::LayerTreeCoordinatorProxy::CreateDirectlyCompositedImage(key, handle));
    m_directlyCompositedImageRefCounts.add(key, 1);
    return key;
}
