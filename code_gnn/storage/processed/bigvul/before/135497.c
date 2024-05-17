void DownloadAsFile(content::WebContents* web_contents, const GURL& url) {
  content::DownloadManager* dlm = content::BrowserContext::GetDownloadManager(
      web_contents->GetBrowserContext());
  std::unique_ptr<download::DownloadUrlParameters> dl_params(
      content::DownloadRequestUtils::CreateDownloadForWebContentsMainFrame(
          web_contents, url, NO_TRAFFIC_ANNOTATION_YET));

  content::NavigationEntry* entry =
      web_contents->GetController().GetLastCommittedEntry();
  DCHECK(entry);
  content::Referrer referrer =
      content::Referrer::SanitizeForRequest(url, entry->GetReferrer());
  dl_params->set_referrer(referrer.url);
  dl_params->set_referrer_policy(content::Referrer::ReferrerPolicyForUrlRequest(
      referrer.policy));

  dl_params->set_prefer_cache(true);
  dl_params->set_prompt(false);
  dl_params->set_download_source(download::DownloadSource::OFFLINE_PAGE);
  dlm->DownloadUrl(std::move(dl_params));
}
