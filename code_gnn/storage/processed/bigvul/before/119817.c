void NavigationControllerImpl::RendererDidNavigateToNewPage(
    const ViewHostMsg_FrameNavigate_Params& params, bool replace_entry) {
  NavigationEntryImpl* new_entry;
  bool update_virtual_url;
  if (pending_entry_ &&
      (!pending_entry_->site_instance() ||
       pending_entry_->site_instance() == web_contents_->GetSiteInstance())) {
    new_entry = new NavigationEntryImpl(*pending_entry_);

    new_entry->set_page_type(PAGE_TYPE_NORMAL);
    update_virtual_url = new_entry->update_virtual_url_with_url();
  } else {
    new_entry = new NavigationEntryImpl;

    GURL url = params.url;
    bool needs_update = false;
    BrowserURLHandlerImpl::GetInstance()->RewriteURLIfNecessary(
        &url, browser_context_, &needs_update);
    new_entry->set_update_virtual_url_with_url(needs_update);

    update_virtual_url = needs_update;
  }

  new_entry->SetURL(params.url);
  if (update_virtual_url)
    UpdateVirtualURLToURL(new_entry, params.url);
  new_entry->SetReferrer(params.referrer);
  new_entry->SetPageID(params.page_id);
  new_entry->SetTransitionType(params.transition);
  new_entry->set_site_instance(
      static_cast<SiteInstanceImpl*>(web_contents_->GetSiteInstance()));
  new_entry->SetHasPostData(params.is_post);
  new_entry->SetPostID(params.post_id);
  new_entry->SetOriginalRequestURL(params.original_request_url);
  new_entry->SetIsOverridingUserAgent(params.is_overriding_user_agent);

  DCHECK(!params.history_list_was_cleared || !replace_entry);
  if (params.history_list_was_cleared) {
    DiscardNonCommittedEntriesInternal();
    entries_.clear();
    last_committed_entry_index_ = -1;
  }

  InsertOrReplaceEntry(new_entry, replace_entry);
}
