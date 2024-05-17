void OmniboxPopupViewGtk::Show(size_t num_results) {
  gint origin_x, origin_y;
  GdkWindow* gdk_window = gtk_widget_get_window(location_bar_);
  gdk_window_get_origin(gdk_window, &origin_x, &origin_y);
  GtkAllocation allocation;
  gtk_widget_get_allocation(location_bar_, &allocation);

  int horizontal_offset = 1;
  gtk_window_move(GTK_WINDOW(window_),
      origin_x + allocation.x - kBorderThickness + horizontal_offset,
      origin_y + allocation.y + allocation.height - kBorderThickness - 1 +
          kVerticalOffset);
  gtk_widget_set_size_request(window_,
      allocation.width + (kBorderThickness * 2) - (horizontal_offset * 2),
      (num_results * kHeightPerResult) + (kBorderThickness * 2));
  gtk_widget_show(window_);
  StackWindow();
  opened_ = true;
}
