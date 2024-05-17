gfx::Point TabStripGtk::GetTabStripOriginForWidget(GtkWidget* target) {
  int x, y;
  GtkAllocation widget_allocation;
  gtk_widget_get_allocation(widget(), &widget_allocation);
  if (!gtk_widget_translate_coordinates(widget(), target,
      -widget_allocation.x, 0, &x, &y)) {
    if (!gtk_widget_translate_coordinates(
        gtk_widget_get_toplevel(widget()), target, 0, 0, &x, &y)) {
      NOTREACHED();
    }
  }
  if (!gtk_widget_get_has_window(target)) {
    GtkAllocation target_allocation;
    gtk_widget_get_allocation(target, &target_allocation);
    x += target_allocation.x;
    y += target_allocation.y;
  }
  return gfx::Point(x, y);
}
