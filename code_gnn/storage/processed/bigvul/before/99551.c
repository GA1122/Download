 void OnDragDataGetForDownloadItem(GtkSelectionData* selection_data,
                                   guint target_type,
                                  const DownloadItem* download_item) {
  GURL url = net::FilePathToFileURL(download_item->full_path());
  gtk_dnd_util::WriteURLWithName(selection_data, url,
      UTF8ToUTF16(download_item->GetFileName().value()), target_type);
}
