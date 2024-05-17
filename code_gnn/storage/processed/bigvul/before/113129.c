  content::MockDownloadItem& AddItemToManager() {
    DownloadCreateInfo info;

    static const char* kDownloadIdDomain = "Test download id domain";

    int id = next_download_id_;
    ++next_download_id_;
    info.download_id = content::DownloadId(kDownloadIdDomain, id);
    info.request_handle = DownloadRequestHandle();
    download_manager_->CreateDownloadItem(&info);

    DCHECK(mock_download_item_factory_->GetItem(id));
    content::MockDownloadItem& item(*mock_download_item_factory_->GetItem(id));
    ON_CALL(item, GetId())
        .WillByDefault(Return(id));

    return item;
  }
