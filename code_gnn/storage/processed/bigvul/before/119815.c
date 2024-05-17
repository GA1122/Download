void NavigationControllerImpl::RendererDidNavigateNewSubframe(
    const ViewHostMsg_FrameNavigate_Params& params) {
  if (PageTransitionCoreTypeIs(params.transition,
                               PAGE_TRANSITION_AUTO_SUBFRAME)) {
    DiscardNonCommittedEntriesInternal();
    return;
  }

  DCHECK(GetLastCommittedEntry()) << "ClassifyNavigation should guarantee "
                                  << "that a last committed entry exists.";
  NavigationEntryImpl* new_entry = new NavigationEntryImpl(
      *NavigationEntryImpl::FromNavigationEntry(GetLastCommittedEntry()));
  new_entry->SetPageID(params.page_id);
  InsertOrReplaceEntry(new_entry, false);
}
