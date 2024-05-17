void BackgroundLoaderOfflinerTest::OnCanDownload(bool allowed) {
  can_download_callback_called_ = true;
  can_download_ = allowed;
}
