  static gboolean OnRealize(GtkWidget* widget,
                            RenderWidgetHostViewGtk* host_view) {
    host_view->signals_.Connect(gtk_widget_get_toplevel(widget),
                                "configure-event",
                                G_CALLBACK(OnConfigureEvent), host_view);
    return FALSE;
  }
