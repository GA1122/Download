void WebContentsImpl::SaveFrameWithHeaders(const GURL& url,
                                           const Referrer& referrer,
                                           const std::string& headers) {
  if (!GetLastCommittedURL().is_valid())
    return;
  if (delegate_ && delegate_->SaveFrame(url, referrer))
    return;

  bool is_main_frame = (url == GetLastCommittedURL());
  RenderFrameHost* frame_host = GetMainFrame();

  StoragePartition* storage_partition = BrowserContext::GetStoragePartition(
      GetBrowserContext(), frame_host->GetSiteInstance());
  int64_t post_id = -1;
  if (is_main_frame) {
    const NavigationEntry* entry = controller_.GetLastCommittedEntry();
    if (entry)
      post_id = entry->GetPostID();
  }
  auto params = base::MakeUnique<DownloadUrlParameters>(
      url, frame_host->GetProcess()->GetID(),
      frame_host->GetRenderViewHost()->GetRoutingID(),
      frame_host->GetRoutingID(), storage_partition->GetURLRequestContext());
  params->set_referrer(referrer);
  params->set_post_id(post_id);
  if (post_id >= 0)
    params->set_method("POST");
  params->set_prompt(true);

  if (headers.empty()) {
    params->set_prefer_cache(true);
  } else {
    for (const base::StringPiece& key_value :
         base::SplitStringPiece(
             headers, "\n", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL)) {
      std::vector<std::string> pair = base::SplitString(
          key_value, ":", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
      DCHECK_EQ(2ul, pair.size());
      params->add_request_header(pair[0], pair[1]);
    }
  }
  BrowserContext::GetDownloadManager(GetBrowserContext())
      ->DownloadUrl(std::move(params), NO_TRAFFIC_ANNOTATION_YET);
}
