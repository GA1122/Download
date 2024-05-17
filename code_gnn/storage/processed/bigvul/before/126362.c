int BrowserWindowGtk::GetXPositionOfLocationIcon(GtkWidget* relative_to) {
  GtkWidget* location_icon = toolbar_->GetLocationBarView()->
      location_icon_widget();

  GtkAllocation location_icon_allocation;
  gtk_widget_get_allocation(location_icon, &location_icon_allocation);

  int x = 0;
  gtk_widget_translate_coordinates(
      location_icon, relative_to,
      (location_icon_allocation.width + 1) / 2,
      0, &x, NULL);

  if (!gtk_widget_get_has_window(relative_to)) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(relative_to, &allocation);
    x += allocation.x;
  }

  return x;
}
