void AeroPeekWindow::UpdateThumbnail() {
  if (ready_to_update_thumbnail_.IsSignaled())
    CallDwmInvalidateIconicBitmaps(hwnd());
}
