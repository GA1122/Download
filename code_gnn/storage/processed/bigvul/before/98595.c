gboolean DraggedTabGtk::OnExposeEvent(GtkWidget* widget,
                                      GdkEventExpose* event,
                                      DraggedTabGtk* dragged_tab) {
  cairo_surface_t* surface = dragged_tab->renderer_->PaintToSurface();
  if (gtk_util::IsScreenComposited()) {
    dragged_tab->SetContainerTransparency();
  } else {
    dragged_tab->SetContainerShapeMask(surface);
  }

  int tab_width = static_cast<int>(kScalingFactor *
      dragged_tab->renderer_->width());
  int tab_height = static_cast<int>(kScalingFactor *
      dragged_tab->renderer_->height());

  BackingStore* backing_store =
      dragged_tab->data_source_->render_view_host()->GetBackingStore(false);
  if (backing_store && !dragged_tab->attached_) {
    static_cast<BackingStoreX*>(backing_store)->PaintToRect(
        gfx::Rect(kDragFrameBorderSize, tab_height,
                  widget->allocation.width - kTwiceDragFrameBorderSize,
                  widget->allocation.height - tab_height -
                  kDragFrameBorderSize),
        GDK_DRAWABLE(widget->window));
  }

  cairo_t* cr = gdk_cairo_create(GDK_DRAWABLE(widget->window));
  if (!dragged_tab->attached_) {
    cairo_set_line_width(cr, kDragFrameBorderSize);
    cairo_set_source_rgb(cr, kDraggedTabBorderColor[0],
                             kDraggedTabBorderColor[1],
                             kDraggedTabBorderColor[2]);
    double offset = kDragFrameBorderSize / 2.0 - 0.5;
    double left_x = offset;
    double top_y = tab_height - kDragFrameBorderSize + offset;
    double right_x = widget->allocation.width - offset;
    double bottom_y = widget->allocation.height - offset;
    double middle_x = tab_width + offset;

    cairo_move_to(cr, left_x, top_y);
    cairo_line_to(cr, left_x, bottom_y);
    cairo_line_to(cr, right_x, bottom_y);
    cairo_line_to(cr, right_x, top_y);
    cairo_line_to(cr, middle_x, top_y);
    cairo_stroke(cr);
  }

  if (!dragged_tab->attached_)
    cairo_scale(cr, kScalingFactor, kScalingFactor);
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);

  cairo_destroy(cr);

  cairo_surface_destroy(surface);

  return TRUE;
}
