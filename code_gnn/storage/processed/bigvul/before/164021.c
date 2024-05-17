void DownloadManagerImpl::CreateSavePackageDownloadItem(
    const base::FilePath& main_file_path,
    const GURL& page_url,
    const std::string& mime_type,
    int render_process_id,
    int render_frame_id,
    std::unique_ptr<download::DownloadRequestHandleInterface> request_handle,
    const DownloadItemImplCreated& item_created) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  GetNextId(
      base::BindOnce(&DownloadManagerImpl::CreateSavePackageDownloadItemWithId,
                     weak_factory_.GetWeakPtr(), main_file_path, page_url,
                     mime_type, render_process_id, render_frame_id,
                     std::move(request_handle), item_created));
}
