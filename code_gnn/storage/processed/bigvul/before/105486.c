static gboolean webkit_web_view_real_move_cursor (WebKitWebView* webView, GtkMovementStep step, gint count)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW (webView), FALSE);
    g_return_val_if_fail(step == GTK_MOVEMENT_VISUAL_POSITIONS ||
                         step == GTK_MOVEMENT_DISPLAY_LINES ||
                         step == GTK_MOVEMENT_PAGES ||
                         step == GTK_MOVEMENT_BUFFER_ENDS, FALSE);
    g_return_val_if_fail(count == 1 || count == -1, FALSE);

    ScrollDirection direction;
    ScrollGranularity granularity;

    switch (step) {
    case GTK_MOVEMENT_DISPLAY_LINES:
        granularity = ScrollByLine;
        if (count == 1)
            direction = ScrollDown;
        else
            direction = ScrollUp;
        break;
    case GTK_MOVEMENT_VISUAL_POSITIONS:
        granularity = ScrollByLine;
        if (count == 1)
            direction = ScrollRight;
        else
            direction = ScrollLeft;
        break;
    case GTK_MOVEMENT_PAGES:
        granularity = ScrollByPage;
        if (count == 1)
            direction = ScrollDown;
        else
            direction = ScrollUp;
        break;
    case GTK_MOVEMENT_BUFFER_ENDS:
        granularity = ScrollByDocument;
        if (count == 1)
            direction = ScrollDown;
        else
            direction = ScrollUp;
        break;
    default:
        g_assert_not_reached();
        return false;
    }

    Frame* frame = core(webView)->focusController()->focusedOrMainFrame();
    if (!frame->eventHandler()->scrollOverflow(direction, granularity))
        frame->view()->scroll(direction, granularity);

    return true;
}
