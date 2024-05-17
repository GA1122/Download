void LayerWebKitThread::paintContents(BlackBerry::Platform::Graphics::Buffer* buffer, const IntRect& contentsRect, double scale)
{
    if (!drawsContent() && !contents())
        return;

    if (!buffer)
        return;

    IntRect untransformedContentsRect = contentsRect;
    FloatRect clipRect = contentsRect;
    if (scale != 1.0) {
        TransformationMatrix matrix;
        matrix.scale(1.0 / scale);
        untransformedContentsRect = matrix.mapRect(contentsRect);
        clipRect = matrix.mapRect(clipRect);

        const int atLeastOneDevicePixel = static_cast<int>(ceilf(1.0 / scale));
        untransformedContentsRect.inflate(atLeastOneDevicePixel);
    }

    PlatformGraphicsContext* platformContext = lockBufferDrawable(buffer);
    GraphicsContext graphicsContext(platformContext);
    if (contents()) {
        if (!contents()->size().isEmpty())
            graphicsContext.drawImage(contents(), ColorSpaceDeviceRGB, IntPoint(0, 0));
    } else {
        graphicsContext.translate(-contentsRect.x(), -contentsRect.y());
        graphicsContext.scale(FloatSize(scale, scale));
        graphicsContext.clip(clipRect);
        m_owner->paintGraphicsLayerContents(graphicsContext, untransformedContentsRect);
    }

    releaseBufferDrawable(buffer);
}
