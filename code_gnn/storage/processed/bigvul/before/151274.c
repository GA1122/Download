Response InspectorPageAgent::navigate(const String& url,
                                      Maybe<String> referrer,
                                      Maybe<String> transitionType,
                                      String* out_frame_id) {
  *out_frame_id = IdentifiersFactory::FrameId(inspected_frames_->Root());
  return Response::OK();
}
