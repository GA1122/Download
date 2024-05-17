void DownloadFileManager::CreateDownloadFile(
    scoped_ptr<DownloadCreateInfo> info,
    scoped_ptr<content::ByteStreamReader> stream,
    scoped_refptr<DownloadManager> download_manager, bool get_hash,
    const net::BoundNetLog& bound_net_log,
    const CreateDownloadFileCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  DCHECK(info.get());
  VLOG(20) << __FUNCTION__ << "()" << " info = " << info->DebugString();

  scoped_ptr<DownloadFile> download_file(download_file_factory_->CreateFile(
      info.get(), stream.Pass(), download_manager, get_hash, bound_net_log));

  content::DownloadInterruptReason interrupt_reason(
      content::ConvertNetErrorToInterruptReason(
          download_file->Initialize(), content::DOWNLOAD_INTERRUPT_FROM_DISK));
  if (interrupt_reason == content::DOWNLOAD_INTERRUPT_REASON_NONE) {
    DCHECK(GetDownloadFile(info->download_id) == NULL);
    downloads_[info->download_id] = download_file.release();
  }

  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                          base::Bind(callback, interrupt_reason));
}
