content::DownloadItem* MockDownloadItemFactory::CreateSavePageItem(
    DownloadItemImpl::Delegate* delegate,
    const FilePath& path,
    const GURL& url,
    bool is_otr,
    content::DownloadId download_id,
    const std::string& mime_type,
    const net::BoundNetLog& bound_net_log) {
  int local_id = download_id.local();
  DCHECK(items_.find(local_id) == items_.end());

  content::MockDownloadItem* result =
      new StrictMock<content::MockDownloadItem>;
  EXPECT_CALL(*result, GetId())
      .WillRepeatedly(Return(local_id));
  items_[local_id] = result;

  return result;
}
