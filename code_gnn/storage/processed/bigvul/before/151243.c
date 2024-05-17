void InspectorPageAgent::FrameStartedLoading(LocalFrame* frame, FrameLoadType) {
  GetFrontend()->frameStartedLoading(IdentifiersFactory::FrameId(frame));
}
