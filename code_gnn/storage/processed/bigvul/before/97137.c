void NavigationController::RendererDidNavigateInPage(
    const ViewHostMsg_FrameNavigate_Params& params, bool* did_replace_entry) {
  DCHECK(PageTransition::IsMainFrame(params.transition)) <<
      "WebKit should only tell us about in-page navs for the main frame.";
  NavigationEntry* existing_entry = GetEntryWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);

  NavigationEntry* new_entry = new NavigationEntry(*existing_entry);
  new_entry->set_page_id(params.page_id);
  new_entry->set_url(params.url);
  *did_replace_entry = IsRedirect(params) &&
                       IsLikelyAutoNavigation(base::TimeTicks::Now());
  InsertOrReplaceEntry(new_entry, *did_replace_entry);
}
