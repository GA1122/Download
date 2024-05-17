static void webkit_web_view_base_init(WebKitWebViewBase* webkitWebViewBase)
{
    WebKitWebViewBasePrivate* priv = G_TYPE_INSTANCE_GET_PRIVATE(webkitWebViewBase, WEBKIT_TYPE_WEB_VIEW_BASE, WebKitWebViewBasePrivate);
    webkitWebViewBase->priv = priv;
    new (priv) WebKitWebViewBasePrivate();

    priv->isPageActive = TRUE;
    priv->shouldForwardNextKeyEvent = FALSE;

    GtkWidget* viewWidget = GTK_WIDGET(webkitWebViewBase);
    gtk_widget_set_double_buffered(viewWidget, FALSE);
    gtk_widget_set_can_focus(viewWidget, TRUE);
    priv->imContext = adoptGRef(gtk_im_multicontext_new());

    priv->pageClient = PageClientImpl::create(viewWidget);

    priv->dragAndDropHelper.setWidget(viewWidget);

    gtk_drag_dest_set(viewWidget, static_cast<GtkDestDefaults>(0), 0, 0,
                      static_cast<GdkDragAction>(GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK | GDK_ACTION_PRIVATE));
    gtk_drag_dest_set_target_list(viewWidget, PasteboardHelper::defaultPasteboardHelper()->targetList());
}
