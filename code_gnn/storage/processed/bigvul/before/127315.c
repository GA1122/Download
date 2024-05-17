    static void configueCompositingWebView(WebSettings* settings)
    {
        settings->setForceCompositingMode(true);
        settings->setAcceleratedCompositingEnabled(true);
        settings->setAcceleratedCompositingForFixedPositionEnabled(true);
        settings->setAcceleratedCompositingForOverflowScrollEnabled(true);
        settings->setAcceleratedCompositingForScrollableFramesEnabled(true);
        settings->setCompositedScrollingForFramesEnabled(true);
        settings->setFixedPositionCreatesStackingContext(true);
    }
