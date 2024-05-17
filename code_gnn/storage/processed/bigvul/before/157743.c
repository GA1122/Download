std::unique_ptr<WebContents> WebContents::CreateWithSessionStorage(
    const WebContents::CreateParams& params,
    const SessionStorageNamespaceMap& session_storage_namespace_map) {
  std::unique_ptr<WebContentsImpl> new_contents(
      new WebContentsImpl(params.browser_context));
  RenderFrameHostImpl* opener_rfh = FindOpenerRFH(params);
  FrameTreeNode* opener = nullptr;
  if (opener_rfh)
    opener = opener_rfh->frame_tree_node();
  new_contents->SetOpenerForNewContents(opener, params.opener_suppressed);

  for (SessionStorageNamespaceMap::const_iterator it =
           session_storage_namespace_map.begin();
       it != session_storage_namespace_map.end();
       ++it) {
    new_contents->GetController()
        .SetSessionStorageNamespace(it->first, it->second.get());
  }

  if (params.guest_delegate) {
    BrowserPluginGuest::Create(new_contents.get(), params.guest_delegate);
  }

  new_contents->Init(params);
  return new_contents;
}
