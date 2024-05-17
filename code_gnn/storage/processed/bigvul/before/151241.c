void InspectorPageAgent::FrameDetachedFromParent(LocalFrame* frame) {
  GetFrontend()->frameDetached(IdentifiersFactory::FrameId(frame));
}
