gboolean TabStripGtk::DropInfo::OnExposeEvent(GtkWidget* widget,
                                              GdkEventExpose* event) {
  TRACE_EVENT0("ui::gtk", "TabStripGtk::DropInfo::OnExposeEvent");

  if (ui::IsScreenComposited()) {
    SetContainerTransparency();
  } else {
    SetContainerShapeMask();
  }

  cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(widget));
  gdk_cairo_rectangle(cr, &event->area);
  cairo_clip(cr);

  drop_arrow->ToCairo()->SetSource(cr, widget, 0, 0);
  cairo_paint(cr);

  cairo_destroy(cr);

  return FALSE;
}
