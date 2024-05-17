static void webkit_web_view_size_request(GtkWidget* widget, GtkRequisition* requisition)
{
    WebKitWebView* web_view = WEBKIT_WEB_VIEW(widget);
    Frame* coreFrame = core(webkit_web_view_get_main_frame(web_view));
    if (!coreFrame)
        return;

    FrameView* view = coreFrame->view();
    if (!view)
        return;

    requisition->width = view->contentsWidth();
    requisition->height = view->contentsHeight();
}
