void NavigationController::RendererDidNavigateToNewPage(
    const ViewHostMsg_FrameNavigate_Params& params, bool* did_replace_entry) {
  NavigationEntry* new_entry;
  bool update_virtual_url;
  if (pending_entry_) {
    new_entry = new NavigationEntry(*pending_entry_);

    new_entry->set_page_type(content::PAGE_TYPE_NORMAL);
    update_virtual_url = new_entry->update_virtual_url_with_url();
  } else {
    new_entry = new NavigationEntry;
    update_virtual_url = true;
  }

  new_entry->set_url(params.url);
  if (update_virtual_url)
    UpdateVirtualURLToURL(new_entry, params.url);
  new_entry->set_referrer(params.referrer);
  new_entry->set_page_id(params.page_id);
  new_entry->set_transition_type(params.transition);
  new_entry->set_site_instance(tab_contents_->GetSiteInstance());
  new_entry->set_has_post_data(params.is_post);

  InsertOrReplaceEntry(new_entry, *did_replace_entry);
}
