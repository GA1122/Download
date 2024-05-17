  void AddItemToHistory(content::MockDownloadItem& item, int64 db_handle) {
    if (DCHECK_IS_ON())
      EXPECT_CALL(item, IsPersisted())
          .WillOnce(Return(false));

    EXPECT_CALL(item, SetDbHandle(db_handle));
    EXPECT_CALL(item, SetIsPersisted());
    EXPECT_CALL(item, GetDbHandle())
        .WillRepeatedly(Return(db_handle));

    EXPECT_CALL(item, GetWebContents())
        .WillOnce(Return(static_cast<WebContents*>(NULL)));
    EXPECT_CALL(GetMockDownloadManagerDelegate(),
                GetAlternativeWebContentsToNotifyForDownload())
        .WillOnce(Return(static_cast<WebContents*>(NULL)));

    EXPECT_CALL(item, IsInProgress())
        .WillOnce(Return(true));

    EXPECT_CALL(item, AllDataSaved())
        .WillOnce(Return(false));

    download_manager_->OnItemAddedToPersistentStore(item.GetId(), db_handle);
  }
