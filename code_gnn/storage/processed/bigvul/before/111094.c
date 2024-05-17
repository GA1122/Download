void WebPagePrivate::clipToTransformedContentsRect(IntRect& rect) const
{
    rect.intersect(IntRect(IntPoint::zero(), transformedContentsSize()));
}
