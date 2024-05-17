    SynchronousCompositorExternalBeginFrameSource(int routing_id)
    : routing_id_(routing_id),
      registered_(false),
      compositor_(nullptr) {
}
