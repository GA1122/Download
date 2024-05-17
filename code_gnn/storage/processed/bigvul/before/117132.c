static gboolean webkitWebViewBaseDraw(GtkWidget* widget, cairo_t* cr)
{
    DrawingAreaProxy* drawingArea = WEBKIT_WEB_VIEW_BASE(widget)->priv->pageProxy->drawingArea();
    if (!drawingArea)
        return FALSE;

    GdkRectangle clipRect;
    if (!gdk_cairo_get_clip_rectangle(cr, &clipRect))
        return FALSE;

    WebCore::Region unpaintedRegion;  
    static_cast<DrawingAreaProxyImpl*>(drawingArea)->paint(cr, clipRect, unpaintedRegion);

    return FALSE;
}
