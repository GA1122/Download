void webkitWebViewBaseStartDrag(WebKitWebViewBase* webViewBase, const DragData& dragData, PassRefPtr<ShareableBitmap> dragImage)
{
    WebKitWebViewBasePrivate* priv = webViewBase->priv;

    RefPtr<DataObjectGtk> dataObject = adoptRef(dragData.platformData());
    GRefPtr<GtkTargetList> targetList = adoptGRef(PasteboardHelper::defaultPasteboardHelper()->targetListForDataObject(dataObject.get()));
    GOwnPtr<GdkEvent> currentEvent(gtk_get_current_event());
    GdkDragContext* context = gtk_drag_begin(GTK_WIDGET(webViewBase),
                                             targetList.get(),
                                             dragOperationToGdkDragActions(dragData.draggingSourceOperationMask()),
                                             1,  
                                             currentEvent.get());
    priv->dragAndDropHelper.startedDrag(context, dataObject.get());


    priv->clickCounter.reset();

    if (dragImage) {
        RefPtr<cairo_surface_t> image(dragImage->createCairoSurface());
        priv->dragIcon.setImage(image.get());
        priv->dragIcon.useForDrag(context);
    } else
        gtk_drag_set_icon_default(context);
}
