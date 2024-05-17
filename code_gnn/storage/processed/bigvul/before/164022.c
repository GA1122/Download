void DownloadManagerImpl::CreateSavePackageDownloadItemWithId(
    const base::FilePath& main_file_path,
    const GURL& page_url,
    const std::string& mime_type,
    int render_process_id,
    int render_frame_id,
    std::unique_ptr<download::DownloadRequestHandleInterface> request_handle,
    const DownloadItemImplCreated& item_created,
    uint32_t id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_NE(download::DownloadItem::kInvalidId, id);
  DCHECK(!base::ContainsKey(downloads_, id));
  download::DownloadItemImpl* download_item = item_factory_->CreateSavePageItem(
      this, id, main_file_path, page_url, mime_type, std::move(request_handle));
  DownloadItemUtils::AttachInfo(download_item, GetBrowserContext(),
                                WebContentsImpl::FromRenderFrameHostID(
                                    render_process_id, render_frame_id));
  OnDownloadCreated(base::WrapUnique(download_item));
  if (!item_created.is_null())
    item_created.Run(download_item);
}