bool NavigationControllerImpl::IsURLSameDocumentNavigation(
    const GURL& url,
    const url::Origin& origin,
    bool renderer_says_same_document,
    RenderFrameHost* rfh) const {
  RenderFrameHostImpl* rfhi = static_cast<RenderFrameHostImpl*>(rfh);
  GURL last_committed_url;
  if (rfh->GetParent()) {
    last_committed_url = rfhi->frame_tree_node()->current_url();
  } else {
    NavigationEntry* last_committed = GetLastCommittedEntry();
    if (!last_committed)
      return false;
    last_committed_url = last_committed->GetURL();
  }

  WebPreferences prefs = rfh->GetRenderViewHost()->GetWebkitPreferences();
  const url::Origin& committed_origin =
      rfhi->frame_tree_node()->current_origin();
  bool is_same_origin = last_committed_url.is_empty() ||
                        last_committed_url == url::kAboutBlankURL ||
                        last_committed_url.GetOrigin() == url.GetOrigin() ||
                        committed_origin == origin ||
                        !prefs.web_security_enabled ||
                        (prefs.allow_universal_access_from_file_urls &&
                         committed_origin.scheme() == url::kFileScheme);
  if (!is_same_origin && renderer_says_same_document) {
    bad_message::ReceivedBadMessage(rfh->GetProcess(),
                                    bad_message::NC_IN_PAGE_NAVIGATION);
  }
  return is_same_origin && renderer_says_same_document;
}
