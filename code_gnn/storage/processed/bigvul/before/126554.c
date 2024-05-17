gboolean TabStripGtk::OnExpose(GtkWidget* widget, GdkEventExpose* event) {
  TRACE_EVENT0("ui::gtk", "TabStripGtk::OnExpose");

  if (gdk_region_empty(event->region))
    return TRUE;

  GdkRectangle* rects;
  gint num_rects;
  gdk_region_get_rectangles(event->region, &rects, &num_rects);
  qsort(rects, num_rects, sizeof(GdkRectangle), CompareGdkRectangles);
  std::vector<int> tabs_to_repaint;
  if (!IsDragSessionActive() &&
      CanPaintOnlyFavicons(rects, num_rects, &tabs_to_repaint)) {
    PaintOnlyFavicons(event, tabs_to_repaint);
    g_free(rects);
    return TRUE;
  }
  g_free(rects);

  if (active_animation_.get() || drag_controller_.get()) {
    event->area.width = bounds_.width();
  } else {
    event->area.width += event->area.x;
  }

  event->area.x = 0;
  event->area.y = 0;
  event->area.height = bounds_.height();
  gdk_region_union_with_rect(event->region, &event->area);

  gtk_container_propagate_expose(GTK_CONTAINER(tabstrip_.get()),
      newtab_button_->widget(), event);

  TabGtk* selected_tab = NULL;
  int tab_count = GetTabCount();
  for (int i = tab_count - 1; i >= 0; --i) {
    TabGtk* tab = GetTabAt(i);
    if (!tab->IsActive()) {
      gtk_container_propagate_expose(GTK_CONTAINER(tabstrip_.get()),
                                     tab->widget(), event);
    } else {
      selected_tab = tab;
    }
  }

  if (selected_tab) {
    gtk_container_propagate_expose(GTK_CONTAINER(tabstrip_.get()),
                                   selected_tab->widget(), event);
  }

  return TRUE;
}
