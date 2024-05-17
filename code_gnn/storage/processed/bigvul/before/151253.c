void InspectorPageAgent::PaintTiming(Document* document,
                                     const char* name,
                                     double timestamp) {
  GetFrontend()->lifecycleEvent(
      IdentifiersFactory::FrameId(document->GetFrame()), name, timestamp);
}
