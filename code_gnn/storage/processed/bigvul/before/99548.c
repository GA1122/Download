void CustomDrag::OnDragBegin(GtkWidget* widget, GdkDragContext* drag_context) {
  if (pixbuf_)
    gtk_drag_set_icon_pixbuf(drag_context, pixbuf_, 0, 0);
}
