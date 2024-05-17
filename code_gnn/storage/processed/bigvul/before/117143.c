void webkitWebViewBaseInitializeFullScreenClient(WebKitWebViewBase* webkitWebViewBase, const WKFullScreenClientGtk* wkClient)
{
    webkitWebViewBase->priv->fullScreenClient.initialize(wkClient);
}
