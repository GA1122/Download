void AutocompletePopupViewGtk::Show(size_t num_results) {
  gint origin_x, origin_y;
  gdk_window_get_origin(location_bar_->window, &origin_x, &origin_y);
  GtkAllocation allocation = location_bar_->allocation;
  int vertical_offset = 0;
  int horizontal_offset = 0;
  if (theme_provider_->UseGtkTheme()) {
    horizontal_offset = kGtkHorizontalOffset;

    vertical_offset = kGtkVerticalOffset;
  }

  gtk_window_move(GTK_WINDOW(window_),
      origin_x + allocation.x - kBorderThickness + horizontal_offset,
      origin_y + allocation.y + allocation.height - kBorderThickness - 1 +
          vertical_offset);
  gtk_widget_set_size_request(window_,
      allocation.width + (kBorderThickness * 2) - (horizontal_offset * 2),
      (num_results * kHeightPerResult) + (kBorderThickness * 2));
   gtk_widget_show(window_);
   StackWindow();
   opened_ = true;
}
