void CustomDrag::OnDragEnd(GtkWidget* widget, GdkDragContext* drag_context) {
  delete this;
}
