IntSize WebPagePrivate::mapToTransformed(const IntSize& size) const
{
    return mapToTransformed(IntRect(IntPoint::zero(), size)).size();
}
