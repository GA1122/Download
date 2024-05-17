HICON AeroPeekWindow::OnGetIcon(UINT index) {
  if (favicon_bitmap_.isNull()) {
    if (!frame_icon_) {
      frame_icon_ = GetAppIcon();
    }
    return frame_icon_;
  }

  favicon_.Set(IconUtil::CreateHICONFromSkBitmap(favicon_bitmap_));
  return favicon_.Get();
}
