void InspectorPageAgent::DidResizeMainFrame() {
  if (!inspected_frames_->Root()->IsMainFrame())
    return;
#if !defined(OS_ANDROID)
  PageLayoutInvalidated(true);
#endif
  GetFrontend()->frameResized();
}
