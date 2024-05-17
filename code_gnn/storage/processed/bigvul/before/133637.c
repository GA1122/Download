void WebContentsImpl::UpdateState(RenderViewHost* rvh,
                                  int32 page_id,
                                  const PageState& page_state) {
  if (rvh != GetRenderViewHost() &&
      !GetRenderManager()->IsRVHOnSwappedOutList(
          static_cast<RenderViewHostImpl*>(rvh)))
    return;


  int entry_index = controller_.GetEntryIndexWithPageID(
      rvh->GetSiteInstance(), page_id);
  if (entry_index < 0)
    return;
  NavigationEntry* entry = controller_.GetEntryAtIndex(entry_index);

  if (page_state == entry->GetPageState())
    return;   
  entry->SetPageState(page_state);
  controller_.NotifyEntryChanged(entry, entry_index);
}
