double WebPagePrivate::maxBlockZoomScale() const
{
    return std::min(maximumBlockZoomScale, maximumScale());
}
