void WebContentsImpl::UpdateTitle(RenderFrameHost* render_frame_host,
                                  int32_t page_id,
                                  const base::string16& title,
                                  base::i18n::TextDirection title_direction) {
  SetNotWaitingForResponse();

  NavigationEntryImpl* entry = controller_.GetEntryWithPageID(
      render_frame_host->GetSiteInstance(), page_id);

  NavigationEntryImpl* new_entry = controller_.GetEntryWithUniqueID(
      static_cast<RenderFrameHostImpl*>(render_frame_host)->nav_entry_id());
  DCHECK_EQ(entry, new_entry);

  if (!entry && render_frame_host != GetMainFrame())
    return;

  if (!UpdateTitleForEntry(entry, title))
    return;

  if (entry == controller_.GetEntryAtOffset(0))
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TITLE);
}
