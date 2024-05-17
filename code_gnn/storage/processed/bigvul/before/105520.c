static void webkit_web_view_set_window_features(WebKitWebView* webView, WebKitWebWindowFeatures* webWindowFeatures)
{
    if (!webWindowFeatures)
      return;
    if (webkit_web_window_features_equal(webView->priv->webWindowFeatures.get(), webWindowFeatures))
      return;
    webView->priv->webWindowFeatures = webWindowFeatures;
}
