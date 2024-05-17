static AtkObject* webkit_web_view_get_accessible(GtkWidget* widget)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    if (!core(webView))
        return 0;

    AXObjectCache::enableAccessibility();

    Frame* coreFrame = core(webView)->mainFrame();
    if (!coreFrame)
        return 0;

    Document* doc = coreFrame->document();
    if (!doc)
        return 0;

    AccessibilityObject* rootAccessible = doc->axObjectCache()->rootObject();
    if (!rootAccessible)
        return 0;

    AtkObject* axRoot = rootAccessible->wrapper();
    if (!axRoot || !ATK_IS_OBJECT(axRoot))
        return 0;

    AtkObject* axWebView = atk_object_ref_accessible_child(ATK_OBJECT(axRoot), 0);
    if (!axWebView || !ATK_IS_OBJECT(axWebView))
        return 0;

    g_object_unref(axWebView);
    return axWebView;
}
