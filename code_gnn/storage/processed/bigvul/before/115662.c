void ClientSession::InjectMouseEvent(const MouseEvent& event) {
  if (authenticated_ && !ShouldIgnoreRemoteMouseInput(event)) {
    RecordMouseButtonState(event);
    MouseEvent event_to_inject = event;
    if (event.has_x() && event.has_y()) {
      SkIPoint pos(SkIPoint::Make(event.x(), event.y()));
      const SkISize& screen = capturer_->size_most_recent();
      pos.setX(std::max(0, std::min(screen.width() - 1, pos.x())));
      pos.setY(std::max(0, std::min(screen.height() - 1, pos.y())));
      event_to_inject.set_x(pos.x());
      event_to_inject.set_y(pos.y());

      remote_mouse_pos_ = pos;

      injected_mouse_positions_.push_back(pos);
      if (injected_mouse_positions_.size() > kNumRemoteMousePositions) {
        VLOG(1) << "Injected mouse positions queue full.";
        injected_mouse_positions_.pop_front();
      }
    }
    input_stub_->InjectMouseEvent(event_to_inject);
  }
}
