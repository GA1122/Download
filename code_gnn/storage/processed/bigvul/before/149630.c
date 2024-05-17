void ResourcePrefetchPredictor::OnURLsDeleted(
    history::HistoryService* history_service,
    const history::DeletionInfo& deletion_info) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(initialization_state_ == INITIALIZED);

  if (deletion_info.IsAllHistory())
    DeleteAllUrls();
  else
    DeleteUrls(deletion_info.deleted_rows());
}
