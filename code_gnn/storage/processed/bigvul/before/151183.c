void InspectorNetworkAgent::FrameScheduledClientNavigation(LocalFrame* frame) {
  String frame_id = IdentifiersFactory::FrameId(frame);
  frames_with_scheduled_client_navigation_.insert(frame_id);
  if (!frames_with_scheduled_navigation_.Contains(frame_id)) {
    frame_navigation_initiator_map_.Set(
        frame_id,
        BuildInitiatorObject(frame->GetDocument(), FetchInitiatorInfo()));
  }
}
