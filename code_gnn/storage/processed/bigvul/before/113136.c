  content::MockDownloadItem& GetMockDownloadItem(int id) {
    content::MockDownloadItem* itemp = mock_download_item_factory_->GetItem(id);

    DCHECK(itemp);
    return *itemp;
  }
