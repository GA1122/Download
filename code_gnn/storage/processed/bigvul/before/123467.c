void ImageBitmapFactories::ImageBitmapLoader::didFinishLoading()
{
    if (!m_loader.arrayBufferResult()) {
        rejectPromise();
        return;
    }
    RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create((char*)m_loader.arrayBufferResult()->data(), m_loader.arrayBufferResult()->byteLength());

    OwnPtr<ImageSource> source = adoptPtr(new ImageSource());
    source->setData(sharedBuffer.get(), true);
    RefPtr<NativeImageSkia> imageSkia = source->createFrameAtIndex(0);
    if (!imageSkia) {
        rejectPromise();
        return;
    }

    RefPtr<Image> image = BitmapImage::create(imageSkia);
    if (!image->width() || !image->height()) {
        rejectPromise();
        return;
    }
    if (!m_cropRect.width() && !m_cropRect.height()) {
        m_cropRect = IntRect(IntPoint(), image->size());
    }

    RefPtrWillBeRawPtr<ImageBitmap> imageBitmap = ImageBitmap::create(image.get(), m_cropRect);
    ScriptScope scope(m_scriptState);
    m_resolver->resolve(imageBitmap.release());
    m_factory->didFinishLoading(this);
}
