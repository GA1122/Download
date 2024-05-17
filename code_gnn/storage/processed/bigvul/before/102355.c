void ExtensionHelper::DidDownloadApplicationIcon(ImageResourceFetcher* fetcher,
                                            const SkBitmap& image) {
  pending_app_info_->icons[fetcher->id()].data = image;

  ImageResourceFetcherList::iterator i;
  for (i = app_icon_fetchers_.begin(); i != app_icon_fetchers_.end(); ++i) {
    if (i->get() == fetcher) {
      i->release();
      app_icon_fetchers_.erase(i);
      break;
    }
  }

  MessageLoop::current()->DeleteSoon(FROM_HERE, fetcher);

  if (--pending_app_icon_requests_ > 0)
    return;

  const size_t kMaxIconSize = 1024 * 128;
  size_t actual_icon_size = 0;
  for (size_t i = 0; i < pending_app_info_->icons.size(); ++i) {
    size_t current_size = pending_app_info_->icons[i].data.getSize();
    if (current_size > kMaxIconSize - actual_icon_size) {
      AddErrorToRootConsole(ASCIIToUTF16(
        "Icons are too large. Maximum total size for app icons is 128 KB."));
      return;
    }
    actual_icon_size += current_size;
  }

  Send(new ExtensionHostMsg_InstallApplication(
      routing_id(), *pending_app_info_));
  pending_app_info_.reset(NULL);
}
