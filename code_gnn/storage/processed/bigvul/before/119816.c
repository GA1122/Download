void NavigationControllerImpl::RendererDidNavigateToExistingPage(
    const ViewHostMsg_FrameNavigate_Params& params) {
  DCHECK(PageTransitionIsMainFrame(params.transition));

  int entry_index = GetEntryIndexWithPageID(web_contents_->GetSiteInstance(),
                                            params.page_id);
  DCHECK(entry_index >= 0 &&
         entry_index < static_cast<int>(entries_.size()));
  NavigationEntryImpl* entry = entries_[entry_index].get();

  entry->SetURL(params.url);
  if (entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(entry, params.url);

  if (PageTransitionIsRedirect(params.transition))
    entry->GetFavicon() = FaviconStatus();

  DCHECK(entry->site_instance() == NULL ||
         entry->site_instance() == web_contents_->GetSiteInstance());
  entry->set_site_instance(
      static_cast<SiteInstanceImpl*>(web_contents_->GetSiteInstance()));

  entry->SetHasPostData(params.is_post);
  entry->SetPostID(params.post_id);

  DiscardNonCommittedEntriesInternal();

  last_committed_entry_index_ =
      GetEntryIndexWithPageID(web_contents_->GetSiteInstance(), params.page_id);
}
