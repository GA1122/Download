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
  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("download_web_contents_frame", R"(
        semantics {
          sender: "Save Page Action"
          description:
            "Saves the given frame's URL to the local file system."
          trigger:
            "The user has triggered a save operation on the frame through a "
            "context menu or other mechanism."
          data: "None."
          destination: WEBSITE
        }
        policy {
          cookies_allowed: YES
          cookies_store: "user"
          setting:
            "This feature cannot be disabled by settings, but it's is only "
            "triggered by user request."
          policy_exception_justification: "Not implemented."
        })");
  auto params = base::MakeUnique<DownloadUrlParameters>(
      url, frame_host->GetProcess()->GetID(),
      frame_host->GetRenderViewHost()->GetRoutingID(),
      frame_host->GetRoutingID(), storage_partition->GetURLRequestContext(),
      traffic_annotation);
  params->set_referrer(referrer);
  params->set_post_id(post_id);
  if (post_id >= 0)
    params->set_method("POST");
  params->set_prompt(true);

  if (headers.empty()) {
    params->set_prefer_cache(true);
  } else {
    for (DownloadUrlParameters::RequestHeadersNameValuePair key_value :
         ParseDownloadHeaders(headers)) {
      params->add_request_header(key_value.first, key_value.second);
    }
  }
  BrowserContext::GetDownloadManager(GetBrowserContext())
      ->DownloadUrl(std::move(params));
}
