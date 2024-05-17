DownloadShelf* BrowserView::GetDownloadShelf() {
  DCHECK(browser_->SupportsWindowFeature(Browser::FEATURE_DOWNLOADSHELF));
  if (!download_shelf_.get()) {
    download_shelf_.reset(new DownloadShelfView(browser_.get(), this));
    download_shelf_->set_owned_by_client();
    GetBrowserViewLayout()->set_download_shelf(download_shelf_.get());
  }
  return download_shelf_.get();
}
