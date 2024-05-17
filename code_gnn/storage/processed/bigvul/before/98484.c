LRESULT AeroPeekWindow::OnDwmSendIconicThumbnail(UINT message,
                                                 WPARAM wparam,
                                                 LPARAM lparam) {
  SendMessage(hwnd(), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(title_.c_str()));

  DCHECK(delegate_);

  SkBitmap thumbnail;
  delegate_->GetTabThumbnail(tab_id_, &thumbnail);

  gfx::Size aeropeek_size(HIWORD(lparam), LOWORD(lparam));
  ChromeThread::PostTask(ChromeThread::IO,
                         FROM_HERE,
                         new SendThumbnailTask(hwnd(),
                                               GetContentBounds(),
                                               aeropeek_size,
                                               thumbnail,
                                               &ready_to_update_thumbnail_));
  return 0;
}
