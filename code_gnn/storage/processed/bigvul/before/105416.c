static void webkit_web_view_drag_end(GtkWidget* widget, GdkDragContext* context)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    if (!priv->draggingDataObjects.contains(context))
        return;

    priv->draggingDataObjects.remove(context);

    Frame* frame = core(webView)->focusController()->focusedOrMainFrame();
    if (!frame)
        return;

    GOwnPtr<GdkEvent> event(gdk_event_new(GDK_BUTTON_RELEASE));
    int x, y, xRoot, yRoot;
    GdkModifierType modifiers = static_cast<GdkModifierType>(0);
#ifdef GTK_API_VERSION_2
    GdkDisplay* display = gdk_display_get_default();
    gdk_display_get_pointer(display, 0, &xRoot, &yRoot, &modifiers);
    event->button.window = gdk_display_get_window_at_pointer(display, &x, &y);
#else
    GdkDevice* device = gdk_drag_context_get_device(context);
    event->button.window = gdk_device_get_window_at_position(device, &x, &y);
    gdk_device_get_position(device, 0, &xRoot, &yRoot);
#endif

    if (event->button.window)
        g_object_ref(event->button.window);
    event->button.x = x;
    event->button.y = y;
    event->button.x_root = xRoot;
    event->button.y_root = yRoot;
    event->button.state = modifiers;

    PlatformMouseEvent platformEvent(&event->button);
    frame->eventHandler()->dragSourceEndedAt(platformEvent, gdkDragActionToDragOperation(gdk_drag_context_get_selected_action(context)));
}
