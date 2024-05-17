gboolean TabStripGtk::OnDragMotion(GtkWidget* widget, GdkDragContext* context,
                                   gint x, gint y, guint time) {
  UpdateDropIndex(context, x, y);
  return TRUE;
}
