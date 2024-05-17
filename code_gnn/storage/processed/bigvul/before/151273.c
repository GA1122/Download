Response InspectorPageAgent::getResourceTree(
    std::unique_ptr<protocol::Page::FrameResourceTree>* object) {
  *object = BuildObjectForFrameTree(inspected_frames_->Root());
  return Response::OK();
}
