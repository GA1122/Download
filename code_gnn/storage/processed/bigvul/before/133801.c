void DownloadProtectionService::CheckDownloadUrl(
    const content::DownloadItem& item,
    const CheckDownloadCallback& callback) {
  DCHECK(!item.GetUrlChain().empty());
  scoped_refptr<DownloadUrlSBClient> client(
      new DownloadUrlSBClient(item, callback, ui_manager_, database_manager_));
  BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&DownloadUrlSBClient::StartCheck, client));
}
