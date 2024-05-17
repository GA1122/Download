void OnOfflinePageAcquireFileAccessPermissionDone(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const ScopedJavaGlobalRef<jobject>& j_tab_ref,
    const std::string& origin,
    bool granted) {
  if (!granted)
    return;

  content::WebContents* web_contents = web_contents_getter.Run();
  if (!web_contents)
    return;

  GURL url = web_contents->GetLastCommittedURL();
  if (url.is_empty())
    return;

  if (!offline_pages::OfflinePageUtils::CanDownloadAsOfflinePage(
          url, web_contents->GetContentsMimeType())) {
    DownloadAsFile(web_contents, url);
    return;
  }

  GURL original_url =
      offline_pages::OfflinePageUtils::GetOriginalURLFromWebContents(
          web_contents);
  OfflinePageUtils::CheckDuplicateDownloads(
      chrome::GetBrowserContextRedirectedInIncognito(
          web_contents->GetBrowserContext()),
      url,
      base::Bind(&DuplicateCheckDone, url, original_url, j_tab_ref, origin));
}
