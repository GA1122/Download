void WebContentsImpl::UpdateStateForFrame(RenderFrameHost* render_frame_host,
                                          const PageState& page_state) {
  RenderFrameHostImpl* rfhi =
      static_cast<RenderFrameHostImpl*>(render_frame_host);
  NavigationEntryImpl* entry =
      controller_.GetEntryWithUniqueID(rfhi->nav_entry_id());
  if (!entry)
    return;

  FrameNavigationEntry* frame_entry =
      entry->GetFrameEntry(rfhi->frame_tree_node());
  if (!frame_entry)
    return;

  if (frame_entry->site_instance() != rfhi->GetSiteInstance())
    return;

  if (page_state == frame_entry->page_state())
    return;   

  DCHECK(page_state.IsValid()) << "Shouldn't set an empty PageState.";

  ExplodedPageState exploded_state;
  if (!DecodePageState(page_state.ToEncodedData(), &exploded_state))
    return;

  if (exploded_state.top.document_sequence_number !=
          frame_entry->document_sequence_number() ||
      exploded_state.top.item_sequence_number !=
          frame_entry->item_sequence_number()) {
    return;
  }

  frame_entry->SetPageState(page_state);
  controller_.NotifyEntryChanged(entry);
}
