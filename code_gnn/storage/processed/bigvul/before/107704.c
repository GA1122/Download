void StatusBubbleGtk::MouseMoved(
    const gfx::Point& location, bool left_content) {
  if (left_content && ignore_next_left_content_) {
    ignore_next_left_content_ = false;
    return;
  }

  last_mouse_location_ = location;
  last_mouse_left_content_ = left_content;

  if (!GTK_WIDGET_REALIZED(container_.get()))
    return;

  GtkWidget* parent = gtk_widget_get_parent(container_.get());
  if (!parent || !GTK_WIDGET_REALIZED(parent))
    return;

  int old_y_offset = y_offset_;
  bool old_flip_horizontally = flip_horizontally_;

  if (left_content) {
    SetFlipHorizontally(false);
    y_offset_ = 0;
  } else {
    GtkWidget* toplevel = gtk_widget_get_toplevel(container_.get());
    if (!toplevel || !GTK_WIDGET_REALIZED(toplevel))
      return;

    bool ltr = !base::i18n::IsRTL();

    GtkRequisition requisition;
    gtk_widget_size_request(container_.get(), &requisition);

    gint toplevel_x = 0, toplevel_y = 0;
    gdk_window_get_position(toplevel->window, &toplevel_x, &toplevel_y);
    gfx::Rect parent_rect =
        gtk_util::GetWidgetRectRelativeToToplevel(parent);
    gfx::Rect bubble_rect(
        toplevel_x + parent_rect.x() +
            (ltr ? 0 : parent->allocation.width - requisition.width),
        toplevel_y + parent_rect.y() +
            parent->allocation.height - requisition.height,
        requisition.width,
        requisition.height);

    int left_threshold =
        bubble_rect.x() - bubble_rect.height() - kMousePadding;
    int right_threshold =
        bubble_rect.right() + bubble_rect.height() + kMousePadding;
    int top_threshold = bubble_rect.y() - kMousePadding;

    if (((ltr && location.x() < right_threshold) ||
         (!ltr && location.x() > left_threshold)) &&
        location.y() > top_threshold) {
      if (download_shelf_is_visible_) {
        SetFlipHorizontally(true);
        y_offset_ = 0;
      } else {
        SetFlipHorizontally(false);
        int distance = std::max(ltr ?
                                    location.x() - right_threshold :
                                    left_threshold - location.x(),
                                top_threshold - location.y());
        y_offset_ = std::min(-1 * distance, requisition.height);
      }
    } else {
      SetFlipHorizontally(false);
      y_offset_ = 0;
    }
  }

  if (y_offset_ != old_y_offset || flip_horizontally_ != old_flip_horizontally)
    gtk_widget_queue_resize_no_redraw(parent);
}
