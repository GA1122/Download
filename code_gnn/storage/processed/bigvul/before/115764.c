void SafeBrowsingBlockingPage::NotifySafeBrowsingService(
    SafeBrowsingService* sb_service,
    const UnsafeResourceList& unsafe_resources,
    bool proceed) {
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&SafeBrowsingService::OnBlockingPageDone,
                 sb_service, unsafe_resources, proceed));
}
