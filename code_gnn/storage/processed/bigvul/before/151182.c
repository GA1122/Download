void InspectorNetworkAgent::FrameClearedScheduledNavigation(LocalFrame* frame) {
  String frame_id = IdentifiersFactory::FrameId(frame);
  frames_with_scheduled_navigation_.erase(frame_id);
  if (!frames_with_scheduled_client_navigation_.Contains(frame_id))
    frame_navigation_initiator_map_.erase(frame_id);
}
