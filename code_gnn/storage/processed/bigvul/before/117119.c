static gboolean webkitWebViewBaseButtonPressEvent(GtkWidget* widget, GdkEventButton* buttonEvent)
{
    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
    WebKitWebViewBasePrivate* priv = webViewBase->priv;
    gtk_widget_grab_focus(widget);

    if (!priv->clickCounter.shouldProcessButtonEvent(buttonEvent))
        return TRUE;

    if (buttonEvent->button == 3)
        priv->contextMenuEvent.set(gdk_event_copy(reinterpret_cast<GdkEvent*>(buttonEvent)));
    priv->pageProxy->handleMouseEvent(NativeWebMouseEvent(reinterpret_cast<GdkEvent*>(buttonEvent),
                                                     priv->clickCounter.clickCountForGdkButtonEvent(widget, buttonEvent)));
    return TRUE;
}
