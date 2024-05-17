void NotifyRouteChangesOnIO(
    const FrameCallback& frame_callback,
    std::unique_ptr<std::set<GlobalFrameRoutingId>> routing_ids) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  ResourceDispatcherHostImpl* rdh = ResourceDispatcherHostImpl::Get();
  if (!rdh)
    return;
  for (const auto& routing_id : *routing_ids)
    frame_callback.Run(rdh, routing_id);
}
