void WebContentsImpl::UpdateTitle(RenderFrameHost* render_frame_host,
                                  const base::string16& title,
                                  base::i18n::TextDirection title_direction) {
  SetNotWaitingForResponse();

  NavigationEntryImpl* entry = controller_.GetEntryWithUniqueID(
      static_cast<RenderFrameHostImpl*>(render_frame_host)->nav_entry_id());

  if (!entry && render_frame_host != GetMainFrame())
    return;

  UpdateTitleForEntry(entry, title);
}
