void BookmarkDrag::OnDragDataGet(GtkWidget* widget, GdkDragContext* context,
                                 GtkSelectionData* selection_data,
                                 guint target_type, guint time) {
  bookmark_utils::WriteBookmarksToSelection(nodes_, selection_data,
                                            target_type, profile_);
}
