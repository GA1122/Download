static AtkObject* webkitWebViewBaseGetAccessible(GtkWidget* widget)
{
    WebKitWebViewBasePrivate* priv = WEBKIT_WEB_VIEW_BASE(widget)->priv;
    if (priv->accessible && atk_socket_is_occupied(ATK_SOCKET(priv->accessible.get())))
        return priv->accessible.get();

    if (!priv->accessible) {
        priv->accessible = adoptGRef(ATK_OBJECT(webkitWebViewBaseAccessibleNew(widget)));

        GtkWidget* parentWidget = gtk_widget_get_parent(widget);
        AtkObject* axParent = parentWidget ? gtk_widget_get_accessible(parentWidget) : 0;
        if (axParent)
            atk_object_set_parent(priv->accessible.get(), axParent);
    }

    String plugID = priv->pageProxy->accessibilityPlugID();
    if (plugID.isNull())
        return priv->accessible.get();

    atk_socket_embed(ATK_SOCKET(priv->accessible.get()), const_cast<gchar*>(plugID.utf8().data()));

    return priv->accessible.get();
}
