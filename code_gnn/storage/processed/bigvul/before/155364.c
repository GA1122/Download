ChromeContentBrowserClient::GetSafeBrowsingUrlCheckerDelegate(
    content::ResourceContext* resource_context) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  ProfileIOData* io_data = ProfileIOData::FromResourceContext(resource_context);
  if (!io_data->safe_browsing_enabled()->GetValue())
    return nullptr;

  if (safe_browsing_service_ && !safe_browsing_url_checker_delegate_) {
    safe_browsing_url_checker_delegate_ =
        base::MakeRefCounted<safe_browsing::UrlCheckerDelegateImpl>(
            safe_browsing_service_->database_manager(),
            safe_browsing_service_->ui_manager());
  }

  return safe_browsing_url_checker_delegate_.get();
}
