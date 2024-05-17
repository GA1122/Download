void LookupRenderFrameHostOrProxy(int process_id,
                                  int routing_id,
                                  RenderFrameHostImpl** rfh,
                                  RenderFrameProxyHost** rfph) {
  *rfh = RenderFrameHostImpl::FromID(process_id, routing_id);
  if (*rfh == nullptr)
    *rfph = RenderFrameProxyHost::FromID(process_id, routing_id);
}
