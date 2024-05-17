static void webkitWebViewBaseContainerForall(GtkContainer* container, gboolean includeInternals, GtkCallback callback, gpointer callbackData)
{
    WebKitWebViewBase* webView = WEBKIT_WEB_VIEW_BASE(container);
    WebKitWebViewBasePrivate* priv = webView->priv;

    WebKitWebViewChildrenMap children = priv->children;
    WebKitWebViewChildrenMap::const_iterator end = children.end();
    for (WebKitWebViewChildrenMap::const_iterator current = children.begin(); current != end; ++current)
        (*callback)(current->first, callbackData);

    if (includeInternals && priv->inspectorView)
        (*callback)(priv->inspectorView, callbackData);
}
