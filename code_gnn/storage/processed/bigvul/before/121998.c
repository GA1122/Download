void AppListController::UpdateArrowPositionAndAnchorPoint(
    const gfx::Point& cursor) {
  gfx::Screen* screen =
      gfx::Screen::GetScreenFor(current_view_->GetWidget()->GetNativeView());
  gfx::Display display = screen->GetDisplayNearestPoint(cursor);

  current_view_->SetBubbleArrow(views::BubbleBorder::FLOAT);
  current_view_->SetAnchorPoint(FindAnchorPoint(display, cursor));
}
