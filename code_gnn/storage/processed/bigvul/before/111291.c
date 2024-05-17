Platform::IntSize WebPagePrivate::recomputeVirtualViewportFromViewportArguments()
{
    static const ViewportArguments defaultViewportArguments;
    if (m_viewportArguments == defaultViewportArguments)
        return IntSize();

    int desktopWidth = DEFAULT_MAX_LAYOUT_WIDTH;
    int deviceWidth = Platform::Graphics::Screen::primaryScreen()->width();
    int deviceHeight = Platform::Graphics::Screen::primaryScreen()->height();
    float devicePixelRatio = m_webSettings->devicePixelRatio();
    ViewportAttributes result = computeViewportAttributes(m_viewportArguments, desktopWidth, deviceWidth, deviceHeight, devicePixelRatio, m_defaultLayoutSize);
    m_page->setDeviceScaleFactor(devicePixelRatio);

    setUserScalable(m_webSettings->isUserScalable() && result.userScalable);
    if (result.initialScale > 0)
        setInitialScale(result.initialScale * devicePixelRatio);
    if (result.minimumScale > 0)
        setMinimumScale(result.minimumScale * devicePixelRatio);
    if (result.maximumScale > 0)
        setMaximumScale(result.maximumScale * devicePixelRatio);

    return Platform::IntSize(result.layoutSize.width(), result.layoutSize.height());
}
