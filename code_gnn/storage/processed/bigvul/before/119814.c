void NavigationControllerImpl::RendererDidNavigateInPage(
    const ViewHostMsg_FrameNavigate_Params& params, bool* did_replace_entry) {
  DCHECK(PageTransitionIsMainFrame(params.transition)) <<
      "WebKit should only tell us about in-page navs for the main frame.";
  NavigationEntryImpl* existing_entry = GetEntryWithPageID(
      web_contents_->GetSiteInstance(), params.page_id);

  existing_entry->SetURL(params.url);
  if (existing_entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(existing_entry, params.url);

  *did_replace_entry = true;

  DiscardNonCommittedEntriesInternal();

  last_committed_entry_index_ =
      GetEntryIndexWithPageID(web_contents_->GetSiteInstance(), params.page_id);
}
