bool NavigatorImpl::NavigateNewChildFrame(
    RenderFrameHostImpl* render_frame_host,
    const GURL& default_url) {
  NavigationEntryImpl* entry =
      controller_->GetEntryWithUniqueID(render_frame_host->nav_entry_id());
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

  return NavigateToEntry(render_frame_host->frame_tree_node(), *frame_entry,
                         *entry, ReloadType::NONE, false, true, false, nullptr);
}
