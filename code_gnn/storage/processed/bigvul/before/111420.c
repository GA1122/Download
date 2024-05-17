IntSize WebPagePrivate::viewportSize() const
{
    return mapFromTransformed(transformedViewportSize());
}
