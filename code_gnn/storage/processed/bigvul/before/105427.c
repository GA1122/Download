GtkMenu* webkit_web_view_get_context_menu(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

#if ENABLE(CONTEXT_MENUS)
    ContextMenu* menu = core(webView)->contextMenuController()->contextMenu();
    if (!menu)
        return 0;
    return menu->platformDescription();
#else
    return 0;
#endif
}
