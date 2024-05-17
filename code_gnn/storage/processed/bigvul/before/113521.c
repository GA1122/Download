IntSize WebPagePrivate::transformedViewportSize() const
{
    return BlackBerry::Platform::Settings::instance()->applicationSize();
}
