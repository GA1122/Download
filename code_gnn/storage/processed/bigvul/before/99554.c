void DownloadItemDrag::SetSource(GtkWidget* widget, DownloadItem* item) {
  gtk_drag_source_set(widget, GDK_BUTTON1_MASK, NULL, 0,
                      kDownloadItemDragAction);
  gtk_dnd_util::SetSourceTargetListFromCodeMask(widget, kDownloadItemCodeMask);
  g_signal_connect(widget, "drag-data-get",
                   G_CALLBACK(OnDragDataGetStandalone), item);
}
