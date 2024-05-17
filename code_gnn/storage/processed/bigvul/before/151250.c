void InspectorPageAgent::LoadEventFired(LocalFrame* frame) {
  double timestamp = MonotonicallyIncreasingTime();
  if (frame == inspected_frames_->Root())
    GetFrontend()->loadEventFired(timestamp);
  GetFrontend()->lifecycleEvent(IdentifiersFactory::FrameId(frame), "load",
                                timestamp);
}
