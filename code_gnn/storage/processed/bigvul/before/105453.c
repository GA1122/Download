WebKitWebViewViewMode webkit_web_view_get_view_mode(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED);

    Page* page = core(webView);
    Page::ViewMode mode = page->viewMode();

    if (mode == Page::ViewModeFloating)
        return WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING;

    if (mode == Page::ViewModeFullscreen)
        return WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN;

    if (mode == Page::ViewModeMaximized)
        return WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED;

    if (mode == Page::ViewModeMinimized)
        return WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED;

    return WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED;
}
