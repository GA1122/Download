void TabStripGtk::DropInfo::SetContainerColorMap() {
  GdkScreen* screen = gtk_widget_get_screen(container);
  GdkColormap* colormap = gdk_screen_get_rgba_colormap(screen);

  if (!colormap)
    colormap = gdk_screen_get_rgb_colormap(screen);

  gtk_widget_set_colormap(container, colormap);
}
