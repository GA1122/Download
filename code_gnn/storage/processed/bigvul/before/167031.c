bool NavigationControllerImpl::StartHistoryNavigationInNewSubframe(
    RenderFrameHostImpl* render_frame_host,
    const GURL& default_url) {
  NavigationEntryImpl* entry =
      GetEntryWithUniqueID(render_frame_host->nav_entry_id());
  if (!entry)
    return false;

  FrameNavigationEntry* frame_entry =
      entry->GetFrameEntry(render_frame_host->frame_tree_node());
  if (!frame_entry)
    return false;

  bool restoring_different_url = frame_entry->url() != default_url;
  UMA_HISTOGRAM_BOOLEAN("SessionRestore.RestoredSubframeURL",
                        restoring_different_url);
  if (restoring_different_url) {
    const std::string& unique_name =
        render_frame_host->frame_tree_node()->unique_name();
    const char kFramePathPrefix[] = "<!--framePath ";
    if (base::StartsWith(unique_name, kFramePathPrefix,
                         base::CompareCase::SENSITIVE)) {
      UMA_HISTOGRAM_COUNTS("SessionRestore.RestoreSubframeFramePathLength",
                           unique_name.size());
    }
  }

  std::unique_ptr<NavigationRequest> request = CreateNavigationRequest(
      render_frame_host->frame_tree_node(), *entry, frame_entry,
      ReloadType::NONE, false  ,
      true  , nullptr, nullptr);

  if (!request)
    return false;

  render_frame_host->frame_tree_node()->navigator()->Navigate(
      std::move(request), ReloadType::NONE, RestoreType::NONE);

  return true;
}
