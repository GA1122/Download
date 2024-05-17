CustomDrag::~CustomDrag() {
  if (pixbuf_)
    g_object_unref(pixbuf_);
  g_object_unref(drag_widget_);
}
