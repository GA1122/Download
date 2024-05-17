void TabStripGtk::DropInfo::CreateContainer() {
  container = gtk_window_new(GTK_WINDOW_POPUP);
  SetContainerColorMap();
  gtk_widget_set_app_paintable(container, TRUE);
  g_signal_connect(container, "expose-event",
                   G_CALLBACK(OnExposeEventThunk), this);
  gtk_widget_add_events(container, GDK_STRUCTURE_MASK);
  gtk_window_move(GTK_WINDOW(container), 0, 0);
  gtk_window_resize(GTK_WINDOW(container),
                    drop_indicator_width, drop_indicator_height);
  gtk_widget_show_all(container);
}
