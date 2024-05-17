void Compositor::DidReceiveCompositorFrameAck() {
  ++activated_frame_count_;
  for (auto& observer : observer_list_)
    observer.OnCompositingEnded(this);
}
