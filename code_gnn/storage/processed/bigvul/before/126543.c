void TabStripGtk::LayoutNewTabButton(double last_tab_right,
                                     double unselected_width) {
  GtkWidget* toplevel = gtk_widget_get_ancestor(widget(), GTK_TYPE_WINDOW);
  bool is_maximized = false;
  if (toplevel) {
    GdkWindow* gdk_window = gtk_widget_get_window(toplevel);
    is_maximized = (gdk_window_get_state(gdk_window) &
                    GDK_WINDOW_STATE_MAXIMIZED) != 0;
  }

  int y = is_maximized ? 0 : kNewTabButtonVOffset;
  int height = newtab_surface_bounds_.height() + kNewTabButtonVOffset - y;

  gfx::Rect bounds(0, y, newtab_surface_bounds_.width(), height);
  int delta = abs(Round(unselected_width) - TabGtk::GetStandardSize().width());
  if (delta > 1 && !needs_resize_layout_) {
    bounds.set_x(bounds_.width() - newtab_button_->WidgetAllocation().width);
  } else {
    bounds.set_x(Round(last_tab_right - kTabHOffset) + kNewTabButtonHOffset);
  }
  bounds.set_x(gtk_util::MirroredLeftPointForRect(tabstrip_.get(), bounds));

  gtk_fixed_move(GTK_FIXED(tabstrip_.get()), newtab_button_->widget(),
                 bounds.x(), bounds.y());
  gtk_widget_set_size_request(newtab_button_->widget(), bounds.width(),
                              bounds.height());
}
