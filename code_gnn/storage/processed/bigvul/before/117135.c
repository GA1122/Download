static void webkitWebViewBaseFinalize(GObject* gobject)
{
    WebKitWebViewBase* webkitWebViewBase = WEBKIT_WEB_VIEW_BASE(gobject);
    webkitWebViewBase->priv->pageProxy->close();

    webkitWebViewBase->priv->~WebKitWebViewBasePrivate();
    G_OBJECT_CLASS(webkit_web_view_base_parent_class)->finalize(gobject);
}
