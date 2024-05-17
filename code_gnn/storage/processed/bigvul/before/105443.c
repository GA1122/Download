static void webkit_web_view_get_preferred_height(GtkWidget* widget, gint* minimum, gint* natural)
{
    WebKitWebView* web_view = WEBKIT_WEB_VIEW(widget);
    Frame* coreFrame = core(webkit_web_view_get_main_frame(web_view));
    if (!coreFrame)
        return;

    FrameView* view = coreFrame->view();
    if (!view)
        return;

    *minimum = *natural = view->contentsHeight();
}
