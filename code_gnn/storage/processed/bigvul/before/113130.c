content::DownloadItem* MockDownloadItemFactory::CreatePersistedItem(
    DownloadItemImpl::Delegate* delegate,
    content::DownloadId download_id,
    const content::DownloadPersistentStoreInfo& info,
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
