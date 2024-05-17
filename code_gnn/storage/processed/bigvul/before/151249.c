void InspectorPageAgent::LifecycleEvent(LocalFrame* frame,
                                        const char* name,
                                        double timestamp) {
  GetFrontend()->lifecycleEvent(IdentifiersFactory::FrameId(frame), name,
                                timestamp);
}
