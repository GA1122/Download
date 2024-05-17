  bool MediaControlsProgressView::OnMousePressed(const ui::MouseEvent& event) {
  gfx::Point location_in_bar(event.location());
  ConvertPointToTarget(this, this->progress_bar_, &location_in_bar);
  if (!event.IsOnlyLeftMouseButton() ||
      !progress_bar_->GetLocalBounds().Contains(location_in_bar)) {
//   if (!event.IsOnlyLeftMouseButton() || event.y() < kMinClickHeight ||
//       event.y() > kMaxClickHeight) {
      return false;
    }
  
  HandleSeeking(location_in_bar);
//   HandleSeeking(event.location());
    return true;
  }