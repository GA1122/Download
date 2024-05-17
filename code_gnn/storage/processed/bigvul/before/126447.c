void BrowserWindowGtk::UpdateDevToolsSplitPosition() {
  if (!window_has_shown_)
    return;
  GtkAllocation contents_rect;
  gtk_widget_get_allocation(contents_vsplit_, &contents_rect);

  if (devtools_window_->dock_side() == DEVTOOLS_DOCK_SIDE_RIGHT) {
    int split_offset = contents_rect.width -
        devtools_window_->GetWidth(contents_rect.width);
    gtk_paned_set_position(GTK_PANED(contents_hsplit_), split_offset);
  } else {
    int split_offset = contents_rect.height -
        devtools_window_->GetHeight(contents_rect.height);
    gtk_paned_set_position(GTK_PANED(contents_vsplit_), split_offset);
  }
}
