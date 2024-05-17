void webkit_web_view_move_cursor(WebKitWebView* webView, GtkMovementStep step, gint count)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(step == GTK_MOVEMENT_VISUAL_POSITIONS ||
                     step == GTK_MOVEMENT_DISPLAY_LINES ||
                     step == GTK_MOVEMENT_PAGES ||
                     step == GTK_MOVEMENT_BUFFER_ENDS);
    g_return_if_fail(count == 1 || count == -1);

    gboolean handled;
    g_signal_emit(webView, webkit_web_view_signals[MOVE_CURSOR], 0, step, count, &handled);
}
