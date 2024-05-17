bool DevToolsDownloadManagerDelegate::DetermineDownloadTarget(
    content::DownloadItem* item,
    const content::DownloadTargetCallback& callback) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  DevToolsDownloadManagerHelper* download_helper =
      DevToolsDownloadManagerHelper::FromWebContents(item->GetWebContents());

  if (proxy_download_delegate_ && !download_helper)
    return proxy_download_delegate_->DetermineDownloadTarget(item, callback);

  if (!download_helper ||
      download_helper->GetDownloadBehavior() !=
          DevToolsDownloadManagerHelper::DownloadBehavior::ALLOW) {
    base::FilePath empty_path = base::FilePath();
    callback.Run(empty_path,
                 content::DownloadItem::TARGET_DISPOSITION_OVERWRITE,
                 download::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS, empty_path,
                 content::DOWNLOAD_INTERRUPT_REASON_FILE_BLOCKED);
    return true;
  }

  base::FilePath download_path =
      base::FilePath::FromUTF8Unsafe(download_helper->GetDownloadPath());

  FilenameDeterminedCallback filename_determined_callback =
      base::Bind(&DevToolsDownloadManagerDelegate::OnDownloadPathGenerated,
                 base::Unretained(this), item->GetId(), callback);

  PostTaskWithTraits(
      FROM_HERE,
      {base::MayBlock(), base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN,
       base::TaskPriority::USER_VISIBLE},
      base::BindOnce(&DevToolsDownloadManagerDelegate::GenerateFilename,
                     item->GetURL(), item->GetContentDisposition(),
                     item->GetSuggestedFilename(), item->GetMimeType(),
                     download_path, filename_determined_callback));
  return true;
}
