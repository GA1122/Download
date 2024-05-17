void QQuickWebViewFlickablePrivate::didChangeViewportProperties(const WebCore::ViewportArguments& args)
{
    viewportArguments = args;

    if (isTransitioningToNewPage)
        return;

    interactionEngine->applyConstraints(computeViewportConstraints());
}
