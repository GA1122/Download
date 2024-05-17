void WebContentsImpl::OnGetNearestFindResultReply(RenderFrameHostImpl* source,
                                                  int request_id,
                                                  float distance) {
  GetOrCreateFindRequestManager()->OnGetNearestFindResultReply(
      source, request_id, distance);
}
