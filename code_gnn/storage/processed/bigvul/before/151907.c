RenderFrameHostImpl::CreateNavigationRequestForCommit(
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    bool is_same_document,
    NavigationEntryImpl* entry_for_request) {
  bool is_renderer_initiated =
      entry_for_request ? entry_for_request->is_renderer_initiated() : true;
  return NavigationRequest::CreateForCommit(
      frame_tree_node_, this, entry_for_request, params, is_renderer_initiated,
      is_same_document);
}
