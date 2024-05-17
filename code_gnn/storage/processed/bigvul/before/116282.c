void QQuickWebView::reload()
{
    Q_D(QQuickWebView);

    WebFrameProxy* mainFrame = d->webPageProxy->mainFrame();
    if (mainFrame && !mainFrame->unreachableURL().isEmpty() && mainFrame->url() != blankURL()) {
        d->webPageProxy->loadURL(mainFrame->unreachableURL());
        return;
    }

    const bool reloadFromOrigin = true;
    d->webPageProxy->reload(reloadFromOrigin);
}
