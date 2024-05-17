static gboolean webkit_web_view_forward_context_menu_event(WebKitWebView* webView, const PlatformMouseEvent& event)
{
    Page* page = core(webView);
    page->contextMenuController()->clearContextMenu();
    Frame* focusedFrame;
    Frame* mainFrame = page->mainFrame();
    gboolean mousePressEventResult = FALSE;

    if (!mainFrame->view())
        return FALSE;

    mainFrame->view()->setCursor(pointerCursor());
    if (page->frameCount()) {
        HitTestRequest request(HitTestRequest::Active);
        IntPoint point = mainFrame->view()->windowToContents(event.pos());
        MouseEventWithHitTestResults mev = mainFrame->document()->prepareMouseEvent(request, point, event);

        Frame* targetFrame = EventHandler::subframeForHitTestResult(mev);
        if (!targetFrame)
            targetFrame = mainFrame;

        focusedFrame = page->focusController()->focusedOrMainFrame();
        if (targetFrame != focusedFrame) {
            page->focusController()->setFocusedFrame(targetFrame);
            focusedFrame = targetFrame;
        }
    } else
        focusedFrame = mainFrame;

    if (focusedFrame->view() && focusedFrame->eventHandler()->handleMousePressEvent(event))
        mousePressEventResult = TRUE;


    bool handledEvent = focusedFrame->eventHandler()->sendContextMenuEvent(event);
    if (!handledEvent)
        return FALSE;

    ContextMenuController* controller = page->contextMenuController();
    ContextMenu* coreMenu = controller->contextMenu();
    if (!coreMenu)
        return mousePressEventResult;

    WebKitWebSettings* settings = webkit_web_view_get_settings(webView);
    gboolean enableDefaultContextMenu;
    g_object_get(settings, "enable-default-context-menu", &enableDefaultContextMenu, NULL);

    if (!enableDefaultContextMenu)
        return FALSE;

    GtkMenu* menu = GTK_MENU(coreMenu->platformDescription());
    if (!menu)
        return FALSE;

    gtk_container_foreach(GTK_CONTAINER(menu), (GtkCallback)contextMenuConnectActivate, controller);

    g_signal_emit(webView, webkit_web_view_signals[POPULATE_POPUP], 0, menu);

    GOwnPtr<GList> items(gtk_container_get_children(GTK_CONTAINER(menu)));
    if (!items)
        return FALSE;

    WebKitWebViewPrivate* priv = webView->priv;
    priv->currentMenu = menu;
    priv->lastPopupXPosition = event.globalX();
    priv->lastPopupYPosition = event.globalY();

    gtk_menu_popup(menu, 0, 0, &PopupMenuPositionFunc, webView, event.button() + 1, gtk_get_current_event_time());
    return TRUE;
}
