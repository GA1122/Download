QtViewportInteractionEngine::Constraints QQuickWebViewFlickablePrivate::computeViewportConstraints()
{
    Q_Q(QQuickWebView);

    QtViewportInteractionEngine::Constraints newConstraints;
    QSize availableSize = q->boundingRect().size().toSize();

    if (availableSize.isEmpty())
        return newConstraints;

    WebPreferences* wkPrefs = webPageProxy->pageGroup()->preferences();

    wkPrefs->setDeviceDPI(240);
    wkPrefs->setDeviceWidth(480);
    wkPrefs->setDeviceHeight(720);

    int minimumLayoutFallbackWidth = qMax<int>(wkPrefs->layoutFallbackWidth(), availableSize.width());

    WebCore::ViewportAttributes attr = WebCore::computeViewportAttributes(viewportArguments, minimumLayoutFallbackWidth, wkPrefs->deviceWidth(), wkPrefs->deviceHeight(), wkPrefs->deviceDPI(), availableSize);
    WebCore::restrictMinimumScaleFactorToViewportSize(attr, availableSize);
    WebCore::restrictScaleFactorToInitialScaleIfNotUserScalable(attr);

    newConstraints.initialScale = attr.initialScale;
    newConstraints.minimumScale = attr.minimumScale;
    newConstraints.maximumScale = attr.maximumScale;
    newConstraints.devicePixelRatio = attr.devicePixelRatio;
    newConstraints.isUserScalable = !!attr.userScalable;
    newConstraints.layoutSize = attr.layoutSize;

    q->experimental()->viewportInfo()->didUpdateViewportConstraints();

    return newConstraints;
}
