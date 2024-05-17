gfx::Size Browser::GetSizeForNewRenderView(WebContents* web_contents) const {
  gfx::Size size = web_contents->GetContainerBounds().size();
  if (size.IsEmpty() || bookmark_bar_state_ != BookmarkBar::DETACHED)
    return size;
  const NavigationEntry* pending_entry =
      web_contents->GetController().GetPendingEntry();
  if (pending_entry &&
      !search::IsNTPURL(pending_entry->GetVirtualURL(), profile_)) {
    size.Enlarge(
        0, window()->GetRenderViewHeightInsetWithDetachedBookmarkBar());
  }
  return size;
}
