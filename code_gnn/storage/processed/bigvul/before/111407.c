IntSize WebPagePrivate::transformedContentsSize() const
{
    const IntSize untransformedContentsSize = contentsSize();
    const FloatPoint transformedBottomRight = m_transformationMatrix->mapPoint(
        FloatPoint(untransformedContentsSize.width(), untransformedContentsSize.height()));
    return IntSize(floorf(transformedBottomRight.x()), floorf(transformedBottomRight.y()));
}
