void webkit_web_view_set_view_mode(WebKitWebView* webView, WebKitWebViewViewMode mode)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    Page* page = core(webView);

    switch (mode) {
    case WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING:
        page->setViewMode(Page::ViewModeFloating);
        break;
    case WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN:
        page->setViewMode(Page::ViewModeFullscreen);
        break;
    case WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED:
        page->setViewMode(Page::ViewModeMaximized);
        break;
    case WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED:
        page->setViewMode(Page::ViewModeMinimized);
        break;
    default:
        page->setViewMode(Page::ViewModeWindowed);
        break;
    }
}
