gboolean Shell::OnCloseWindowKeyPressed(GtkAccelGroup* accel_group,
                                        GObject* acceleratable,
                                        guint keyval,
                                        GdkModifierType modifier) {
  gtk_widget_destroy(GTK_WIDGET(window_));
  return TRUE;
}
