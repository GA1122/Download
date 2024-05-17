IntSize WebPagePrivate::mapFromTransformed(const IntSize& size) const
{
    return mapFromTransformed(IntRect(IntPoint::zero(), size)).size();
}
