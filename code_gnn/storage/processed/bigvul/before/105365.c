static void PopupMenuPositionFunc(GtkMenu* menu, gint *x, gint *y, gboolean *pushIn, gpointer userData)
{
    WebKitWebView* view = WEBKIT_WEB_VIEW(userData);
    WebKitWebViewPrivate* priv = view->priv;
    GdkScreen* screen = gtk_widget_get_screen(GTK_WIDGET(view));
    GtkRequisition menuSize;

#ifdef GTK_API_VERSION_2
    gtk_widget_size_request(GTK_WIDGET(menu), &menuSize);
#else
    gtk_widget_get_preferred_size(GTK_WIDGET(menu), &menuSize, NULL);
#endif

    *x = priv->lastPopupXPosition;
    if ((*x + menuSize.width) >= gdk_screen_get_width(screen))
      *x -= menuSize.width;

    *y = priv->lastPopupYPosition;
    if ((*y + menuSize.height) >= gdk_screen_get_height(screen))
      *y -= menuSize.height;

    *pushIn = FALSE;
}
