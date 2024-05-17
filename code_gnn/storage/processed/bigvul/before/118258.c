void AutofillDialogViews::Layout() {
  const gfx::Rect content_bounds = GetContentsBounds();
  if (sign_in_web_view_->visible()) {
    sign_in_web_view_->SetBoundsRect(content_bounds);
    return;
  }

  if (loading_shield_->visible()) {
    loading_shield_->SetBoundsRect(bounds());
    return;
  }

  const int x = content_bounds.x();
  const int y = content_bounds.y();
  const int width = content_bounds.width();
  int notification_height = notification_area_->GetHeightForWidth(width);
  notification_area_->SetBounds(x, y, width, notification_height);

  if (scrollable_area_->visible()) {
    int scroll_y = y;
    if (notification_height > notification_area_->GetInsets().height())
      scroll_y += notification_height + views::kRelatedControlVerticalSpacing;

    int scroll_bottom = content_bounds.bottom();
    DCHECK_EQ(scrollable_area_->contents(), details_container_);
    details_container_->SizeToPreferredSize();
    details_container_->Layout();
    details_container_->set_ignore_layouts(true);
    scrollable_area_->SetBounds(x, scroll_y, width, scroll_bottom - scroll_y);
    details_container_->set_ignore_layouts(false);
  }

  if (error_bubble_)
    error_bubble_->UpdatePosition();
}
