void InitializeDownloadTabInfoOnUIThread(
    const DownloadRequestHandle& request_handle,
    DownloadResourceHandler::DownloadTabInfo* tab_info) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  WebContents* web_contents = request_handle.GetWebContents();
  if (web_contents) {
    NavigationEntry* entry = web_contents->GetController().GetVisibleEntry();
    if (entry) {
      tab_info->tab_url = entry->GetURL();
      tab_info->tab_referrer_url = entry->GetReferrer().url;

      tab_info->ukm_source_id = static_cast<WebContentsImpl*>(web_contents)
                                    ->GetUkmSourceIdForLastCommittedSource();
    }
  }
}
