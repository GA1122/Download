  DownloadItem* CreateDownloadItem(DownloadItem::DownloadState state) {
    scoped_ptr<DownloadCreateInfo> info_;

    info_.reset(new DownloadCreateInfo());
    static int next_id;
    info_->download_id =
        content::DownloadId(kValidDownloadItemIdDomain, ++next_id);
    info_->prompt_user_for_save_location = false;
    info_->url_chain.push_back(GURL());
    info_->state = state;

    scoped_ptr<DownloadRequestHandleInterface> request_handle(
        new testing::NiceMock<MockRequestHandle>);
    DownloadItem* download =
        new DownloadItemImpl(&delegate_, *(info_.get()),
                             request_handle.Pass(), false, net::BoundNetLog());
    allocated_downloads_.insert(download);
    return download;
  }
