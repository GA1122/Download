void DownloadItemDrag::OnDragDataGet(
    GtkWidget* widget, GdkDragContext* context,
    GtkSelectionData* selection_data,
    guint target_type, guint time) {
  OnDragDataGetForDownloadItem(selection_data, target_type, download_item_);
}
