void DownloadManagerImpl::GetNextId(GetNextIdCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (IsNextIdInitialized()) {
    std::move(callback).Run(next_download_id_++);
    return;
  }

  id_callbacks_.emplace_back(
      std::make_unique<GetNextIdCallback>(std::move(callback)));
  if (!is_history_download_id_retrieved_ && id_callbacks_.size() == 1u) {
    if (delegate_) {
      delegate_->GetNextId(
          base::BindRepeating(&DownloadManagerImpl::OnHistoryNextIdRetrived,
                              weak_factory_.GetWeakPtr()));
    } else {
      OnHistoryNextIdRetrived(download::DownloadItem::kInvalidId);
    }
  }
}
