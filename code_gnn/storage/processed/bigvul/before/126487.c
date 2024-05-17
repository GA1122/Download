void TabStripGtk::DropInfo::DestroyContainer() {
  if (GTK_IS_WIDGET(container))
    gtk_widget_destroy(container);
}
