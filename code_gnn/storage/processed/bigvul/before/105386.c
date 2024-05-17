void webViewEnterFullscreen(WebKitWebView* webView, Node* node)
{
    if (!node->hasTagName(HTMLNames::videoTag))
        return;

#if ENABLE(VIDEO)
    HTMLMediaElement* videoElement = static_cast<HTMLMediaElement*>(node);
    WebKitWebViewPrivate* priv = webView->priv;

    if (priv->fullscreenVideoController)
        priv->fullscreenVideoController->exitFullscreen();

    priv->fullscreenVideoController = new FullscreenVideoController;
    priv->fullscreenVideoController->setMediaElement(videoElement);
    priv->fullscreenVideoController->enterFullscreen();
#endif
}
