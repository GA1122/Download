void DuplicateCheckDone(const GURL& url,
                        const GURL& original_url,
                        const ScopedJavaGlobalRef<jobject>& j_tab_ref,
                        const std::string& origin,
                        OfflinePageUtils::DuplicateCheckResult result) {
  if (result == OfflinePageUtils::DuplicateCheckResult::NOT_FOUND) {
    SavePageIfNotNavigatedAway(url, original_url, j_tab_ref, origin);
    return;
  }

  content::WebContents* web_contents = GetWebContentsFromJavaTab(j_tab_ref);
  if (!web_contents)
    return;

  bool duplicate_request_exists =
      result == OfflinePageUtils::DuplicateCheckResult::DUPLICATE_REQUEST_FOUND;
  OfflinePageInfoBarDelegate::Create(
      base::Bind(&SavePageIfNotNavigatedAway, url, original_url, j_tab_ref,
                 origin),
      url, duplicate_request_exists, web_contents);
}
