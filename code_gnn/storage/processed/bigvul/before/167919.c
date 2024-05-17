void LocalFrame::DidFreeze() {
  DCHECK(RuntimeEnabledFeatures::PageLifecycleEnabled());
  if (GetDocument()) {
    GetDocument()->DispatchFreezeEvent();
    if (auto* frame_resource_coordinator = GetFrameResourceCoordinator()) {
      frame_resource_coordinator->SetLifecycleState(
          resource_coordinator::mojom::LifecycleState::kFrozen);
    }
  }
}
