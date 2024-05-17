FrameResourceCoordinator* LocalFrame::GetFrameResourceCoordinator() {
  if (!BlinkResourceCoordinatorBase::IsEnabled())
    return nullptr;
  if (!frame_resource_coordinator_) {
    auto* local_frame_client = Client();
    if (!local_frame_client)
      return nullptr;
    frame_resource_coordinator_.reset(FrameResourceCoordinator::Create(
        local_frame_client->GetInterfaceProvider()));
  }
  return frame_resource_coordinator_.get();
}
