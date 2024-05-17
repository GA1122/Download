void RenderThreadImpl::RegisterPendingFrameCreate(
    const service_manager::BindSourceInfo& browser_info,
    int routing_id,
    mojom::FrameRequest frame_request) {
  std::pair<PendingFrameCreateMap::iterator, bool> result =
      pending_frame_creates_.insert(std::make_pair(
          routing_id, base::MakeRefCounted<PendingFrameCreate>(
                          browser_info, routing_id, std::move(frame_request))));
  CHECK(result.second) << "Inserting a duplicate item.";
}
