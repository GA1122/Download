void DownloadManagerImpl::StartDownloadWithId(
    std::unique_ptr<DownloadCreateInfo> info,
    std::unique_ptr<DownloadManager::InputStream> stream,
    const DownloadUrlParameters::OnStartedCallback& on_started,
    bool new_download,
    uint32_t id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_NE(content::DownloadItem::kInvalidId, id);
  DownloadItemImpl* download = nullptr;
  if (new_download) {
    download = CreateActiveItem(id, *info);
  } else {
    auto item_iterator = downloads_.find(id);
    if (item_iterator == downloads_.end() ||
        (item_iterator->second->GetState() == DownloadItem::CANCELLED)) {
      info->request_handle->CancelRequest(true);
      if (!on_started.is_null())
        on_started.Run(nullptr, DOWNLOAD_INTERRUPT_REASON_USER_CANCELED);
      if (info->result == DOWNLOAD_INTERRUPT_REASON_NONE)
        GetDownloadTaskRunner()->DeleteSoon(FROM_HERE, stream.release());
      return;
    }
    download = item_iterator->second.get();
  }

  base::FilePath default_download_directory;
#if defined(USE_X11)
  default_download_directory = GetTemporaryDownloadDirectory();
#else
  if (delegate_) {
    base::FilePath website_save_directory;   
    bool skip_dir_check = false;             
    delegate_->GetSaveDir(GetBrowserContext(), &website_save_directory,
                          &default_download_directory, &skip_dir_check);
  }
#endif

  if (delegate_) {
    if (!in_progress_download_observer_) {
      in_progress_download_observer_.reset(
          new InProgressDownloadObserver(delegate_->GetInProgressCache()));
    }
    download->RemoveObserver(in_progress_download_observer_.get());
    download->AddObserver(in_progress_download_observer_.get());
  }

  std::unique_ptr<DownloadFile> download_file;

  if (info->result == DOWNLOAD_INTERRUPT_REASON_NONE) {
    DCHECK(stream.get());
    download_file.reset(file_factory_->CreateFile(
        std::move(info->save_info), default_download_directory,
        std::move(stream), id, download->DestinationObserverAsWeakPtr()));
  }

  download->Start(std::move(download_file), std::move(info->request_handle),
                  *info);

  if (new_download) {
    for (auto& observer : observers_)
      observer.OnDownloadCreated(this, download);
  }

  if (!on_started.is_null())
    on_started.Run(download, DOWNLOAD_INTERRUPT_REASON_NONE);
}
