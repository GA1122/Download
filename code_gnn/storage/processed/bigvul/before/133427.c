void SVGImage::drawPatternForContainer(GraphicsContext* context, const FloatSize containerSize, float zoom, const FloatRect& srcRect,
    const FloatSize& scale, const FloatPoint& phase, CompositeOperator compositeOp, const FloatRect& dstRect, blink::WebBlendMode blendMode, const IntSize& repeatSpacing)
{
    FloatRect zoomedContainerRect = FloatRect(FloatPoint(), containerSize);
    zoomedContainerRect.scale(zoom);

    AffineTransform transform = context->getCTM();
    FloatSize imageBufferScale = FloatSize(transform.xScale(), transform.yScale());
    ASSERT(imageBufferScale.width());
    ASSERT(imageBufferScale.height());

    FloatSize scaleWithoutCTM(scale.width() / imageBufferScale.width(), scale.height() / imageBufferScale.height());

    FloatRect imageBufferSize = zoomedContainerRect;
    imageBufferSize.scale(imageBufferScale.width(), imageBufferScale.height());

    OwnPtr<ImageBuffer> buffer = ImageBuffer::create(expandedIntSize(imageBufferSize.size()));
    if (!buffer)  
        return;

    drawForContainer(buffer->context(), containerSize, zoom, imageBufferSize, zoomedContainerRect, CompositeSourceOver, blink::WebBlendModeNormal);
    RefPtr<Image> image = buffer->copyImage(DontCopyBackingStore, Unscaled);

    FloatRect scaledSrcRect = srcRect;
    scaledSrcRect.scale(imageBufferScale.width(), imageBufferScale.height());

    image->drawPattern(context, scaledSrcRect, scaleWithoutCTM, phase, compositeOp, dstRect, blendMode, repeatSpacing);
}
