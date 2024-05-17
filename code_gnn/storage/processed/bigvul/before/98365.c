void FrameLoaderClient::transitionToCommittedForNewPage()
{
    WebKitWebView* containingWindow = getViewFromFrame(m_frame);
    IntSize size = IntSize(GTK_WIDGET(containingWindow)->allocation.width,
                           GTK_WIDGET(containingWindow)->allocation.height);
    bool transparent = webkit_web_view_get_transparent(containingWindow);
    Color backgroundColor = transparent ? WebCore::Color::transparent : WebCore::Color::white;
    Frame* frame = core(m_frame);
    ASSERT(frame);

    frame->createView(size, backgroundColor, transparent, IntSize(), false);

    if (frame != frame->page()->mainFrame())
        return;

    WebKitWebViewPrivate* priv = WEBKIT_WEB_VIEW_GET_PRIVATE(containingWindow);
    frame->view()->setGtkAdjustments(priv->horizontalAdjustment, priv->verticalAdjustment);

    if (priv->currentMenu) {
        GtkMenu* menu = priv->currentMenu;
        priv->currentMenu = NULL;

        gtk_menu_popdown(menu);
        g_object_unref(menu);
    }
}
