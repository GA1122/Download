void webViewExitFullscreen(WebKitWebView* webView)
{
#if ENABLE(VIDEO)
    WebKitWebViewPrivate* priv = webView->priv;
    if (priv->fullscreenVideoController)
        priv->fullscreenVideoController->exitFullscreen();
#endif
}
