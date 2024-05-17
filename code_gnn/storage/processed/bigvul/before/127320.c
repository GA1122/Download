static void enableViewportSettings(WebSettings* settings)
{
    settings->setViewportMetaEnabled(true);
    settings->setViewportEnabled(true);
    settings->setMainFrameResizesAreOrientationChanges(true);
}
