static gboolean webkit_web_view_draw(GtkWidget* widget, cairo_t* cr)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;
    GdkRectangle clipRect;

    if (!gdk_cairo_get_clip_rectangle(cr, &clipRect))
        return FALSE;

    Frame* frame = core(webView)->mainFrame();
    if (frame->contentRenderer() && frame->view()) {
        GraphicsContext gc(cr);
        IntRect rect = clipRect;
        cairo_rectangle_list_t* rectList = cairo_copy_clip_rectangle_list(cr);

        frame->view()->updateLayoutAndStyleIfNeededRecursive();

        Vector<IntRect> rects;
        if (!rectList->status && rectList->num_rectangles > 0) {
            for (int i = 0; i < rectList->num_rectangles; i++)
                rects.append(enclosingIntRect(FloatRect(rectList->rectangles[i])));
        }
        paintWebView(frame, priv->transparent, gc, rect, rects);

        cairo_rectangle_list_destroy(rectList);
    }

    return FALSE;
}
