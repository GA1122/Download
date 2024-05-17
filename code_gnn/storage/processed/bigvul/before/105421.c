static gboolean webkit_web_view_expose_event(GtkWidget* widget, GdkEventExpose* event)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    Frame* frame = core(webView)->mainFrame();
    if (frame->contentRenderer() && frame->view()) {
        frame->view()->updateLayoutAndStyleIfNeededRecursive();

        RefPtr<cairo_t> cr = adoptRef(gdk_cairo_create(event->window));
        GraphicsContext gc(cr.get());
        gc.setGdkExposeEvent(event);

        int rectCount;
        GOwnPtr<GdkRectangle> rects;
        gdk_region_get_rectangles(event->region, &rects.outPtr(), &rectCount);
        Vector<IntRect> paintRects;
        for (int i = 0; i < rectCount; i++)
            paintRects.append(IntRect(rects.get()[i]));

        paintWebView(frame, priv->transparent, gc, static_cast<IntRect>(event->area), paintRects);
    }

    return FALSE;
}
