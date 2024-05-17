LRESULT AeroPeekWindow::OnDwmSendIconicLivePreviewBitmap(UINT message,
                                                         WPARAM wparam,
                                                         LPARAM lparam) {
  DCHECK(delegate_);

  SkBitmap preview;
  delegate_->GetTabPreview(tab_id_, &preview);

  ChromeThread::PostTask(ChromeThread::IO,
                         FROM_HERE,
                         new SendLivePreviewTask(hwnd(),
                                                 GetContentBounds(),
                                                 preview));
  return 0;
}
