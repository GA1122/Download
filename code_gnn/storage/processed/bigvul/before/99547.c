DownloadItemDrag::DownloadItemDrag(const DownloadItem* item,
                                   SkBitmap* icon)
    : CustomDrag(icon, kDownloadItemCodeMask, kDownloadItemDragAction),
      download_item_(item) {
}
