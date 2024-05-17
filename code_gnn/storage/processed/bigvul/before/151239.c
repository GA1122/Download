void InspectorPageAgent::FrameAttachedToParent(LocalFrame* frame) {
  Frame* parent_frame = frame->Tree().Parent();
  if (!parent_frame->IsLocalFrame())
    parent_frame = 0;
  std::unique_ptr<SourceLocation> location =
      SourceLocation::CaptureWithFullStackTrace();
  GetFrontend()->frameAttached(
      IdentifiersFactory::FrameId(frame),
      IdentifiersFactory::FrameId(ToLocalFrame(parent_frame)),
      location ? location->BuildInspectorObject() : nullptr);
}
