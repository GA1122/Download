PassRefPtr<NativeImageSkia> SVGImage::nativeImageForCurrentFrame()
{
    if (!m_page)
        return nullptr;

    OwnPtr<ImageBuffer> buffer = ImageBuffer::create(size());
    if (!buffer)
        return nullptr;

    drawForContainer(buffer->context(), size(), 1, rect(), rect(), CompositeSourceOver, blink::WebBlendModeNormal);

    return buffer->copyImage(CopyBackingStore)->nativeImageForCurrentFrame();
}
