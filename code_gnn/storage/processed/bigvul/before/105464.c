static void webkit_web_view_init(WebKitWebView* webView)
{
    WebKitWebViewPrivate* priv = G_TYPE_INSTANCE_GET_PRIVATE(webView, WEBKIT_TYPE_WEB_VIEW, WebKitWebViewPrivate);
    webView->priv = priv;
    new (priv) WebKitWebViewPrivate();

    priv->imContext = adoptGRef(gtk_im_multicontext_new());

    Page::PageClients pageClients;
    pageClients.chromeClient = new WebKit::ChromeClient(webView);
    pageClients.contextMenuClient = new WebKit::ContextMenuClient(webView);
    pageClients.editorClient = new WebKit::EditorClient(webView);
    pageClients.dragClient = new WebKit::DragClient(webView);
    pageClients.inspectorClient = new WebKit::InspectorClient(webView);
    priv->corePage = new Page(pageClients);

    priv->corePage->setGroupName("org.webkit.gtk.WebKitGTK");

    priv->webInspector = adoptGRef(WEBKIT_WEB_INSPECTOR(g_object_new(WEBKIT_TYPE_WEB_INSPECTOR, NULL)));
    webkit_web_inspector_set_inspector_client(priv->webInspector.get(), priv->corePage);

    priv->viewportAttributes = adoptGRef(WEBKIT_VIEWPORT_ATTRIBUTES(g_object_new(WEBKIT_TYPE_VIEWPORT_ATTRIBUTES, NULL)));
    priv->viewportAttributes->priv->webView = webView;

    gtk_widget_set_can_focus(GTK_WIDGET(webView), TRUE);
    priv->mainFrame = WEBKIT_WEB_FRAME(webkit_web_frame_new(webView));
    priv->lastPopupXPosition = priv->lastPopupYPosition = -1;

    priv->backForwardList = adoptGRef(webkit_web_back_forward_list_new_with_web_view(webView));

    priv->zoomFullContent = FALSE;

    priv->webSettings = adoptGRef(webkit_web_settings_new());
    webkit_web_view_update_settings(webView);
    g_signal_connect(priv->webSettings.get(), "notify", G_CALLBACK(webkit_web_view_settings_notify), webView);

    priv->webWindowFeatures = adoptGRef(webkit_web_window_features_new());

    priv->subResources = adoptGRef(g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_object_unref));

    gtk_drag_dest_set(GTK_WIDGET(webView), static_cast<GtkDestDefaults>(0), 0, 0, static_cast<GdkDragAction>(GDK_ACTION_COPY | GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK | GDK_ACTION_PRIVATE));
    gtk_drag_dest_set_target_list(GTK_WIDGET(webView), pasteboardHelperInstance()->targetList());
}
