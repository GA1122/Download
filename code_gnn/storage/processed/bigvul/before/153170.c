void Compositor::RequestPresentationTimeForNextFrame(
    PresentationTimeCallback callback) {
  host_->RequestPresentationTimeForNextFrame(std::move(callback));
}
