void InspectorPageAgent::DomContentLoadedEventFired(LocalFrame* frame) {
  double timestamp = MonotonicallyIncreasingTime();
  if (frame == inspected_frames_->Root())
    GetFrontend()->domContentEventFired(timestamp);
  GetFrontend()->lifecycleEvent(IdentifiersFactory::FrameId(frame),
                                "DOMContentLoaded", timestamp);
}
