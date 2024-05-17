gboolean Shell::OnHighlightURLView(GtkAccelGroup* accel_group,
                                   GObject* acceleratable,
                                   guint keyval,
                                   GdkModifierType modifier) {
  gtk_widget_grab_focus(GTK_WIDGET(url_edit_view_));
  return TRUE;
}
