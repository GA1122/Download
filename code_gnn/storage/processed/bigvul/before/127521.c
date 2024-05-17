IntRect LayerWebKitThread::mapFromTransformed(const IntRect& contentsRect, double scale)
{
    IntRect untransformedContentsRect = contentsRect;

    if (scale != 1.0) {
        TransformationMatrix matrix;
        matrix.scale(1.0 / scale);
        untransformedContentsRect = matrix.mapRect(contentsRect);

        const int atLeastOneDevicePixel = static_cast<int>(ceilf(1.0 / scale));
        untransformedContentsRect.inflate(atLeastOneDevicePixel);
    }

    return untransformedContentsRect;
}
