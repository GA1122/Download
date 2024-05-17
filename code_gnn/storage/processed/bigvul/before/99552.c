void OnDragDataGetStandalone(GtkWidget* widget, GdkDragContext* context,
                             GtkSelectionData* selection_data,
                             guint target_type, guint time,
                             const DownloadItem* item) {
  OnDragDataGetForDownloadItem(selection_data, target_type, item);
}
