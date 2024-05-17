static void webkitWebViewBaseRealize(GtkWidget* widget)
{
    gtk_widget_set_realized(widget, TRUE);

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    GdkWindowAttr attributes;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.x = allocation.x;
    attributes.y = allocation.y;
    attributes.width = allocation.width;
    attributes.height = allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.visual = gtk_widget_get_visual(widget);
    attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
        | GDK_EXPOSURE_MASK
        | GDK_BUTTON_PRESS_MASK
        | GDK_BUTTON_RELEASE_MASK
        | GDK_SCROLL_MASK
        | GDK_POINTER_MOTION_MASK
        | GDK_KEY_PRESS_MASK
        | GDK_KEY_RELEASE_MASK
        | GDK_BUTTON_MOTION_MASK
        | GDK_BUTTON1_MOTION_MASK
        | GDK_BUTTON2_MOTION_MASK
        | GDK_BUTTON3_MOTION_MASK;

    gint attributesMask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

    GdkWindow* window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributesMask);
#if USE(TEXTURE_MAPPER_GL)
    gdk_window_ensure_native(window);
#endif
    gtk_widget_set_window(widget, window);
    gdk_window_set_user_data(window, widget);

    gtk_style_context_set_background(gtk_widget_get_style_context(widget), window);

    WebKitWebViewBase* webView = WEBKIT_WEB_VIEW_BASE(widget);
    WebKitWebViewBasePrivate* priv = webView->priv;
    gtk_im_context_set_client_window(priv->imContext.get(), window);

    GtkWidget* toplevel = gtk_widget_get_toplevel(widget);
    if (widgetIsOnscreenToplevelWindow(toplevel)) {
        webkitWebViewBaseNotifyResizerSizeForWindow(webView, GTK_WINDOW(toplevel));
        g_signal_connect(toplevel, "notify::resize-grip-visible",
                         G_CALLBACK(toplevelWindowResizeGripVisibilityChanged), webView);
    }
}
