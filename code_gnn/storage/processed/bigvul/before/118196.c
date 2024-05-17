gfx::Size AutofillDialogViews::CalculatePreferredSize(
    bool get_minimum_size) const {
  gfx::Insets insets = GetInsets();
  gfx::Size scroll_size = scrollable_area_->contents()->GetPreferredSize();
  const int width = scroll_size.width();

  if (sign_in_web_view_->visible()) {
    const gfx::Size size = static_cast<views::View*>(sign_in_web_view_)->
        GetPreferredSize();
    return gfx::Size(width + insets.width(), size.height() + insets.height());
  }

  if (overlay_view_->visible()) {
    const int height = overlay_view_->GetHeightForContentsForWidth(width);
    if (height != 0)
      return gfx::Size(width + insets.width(), height + insets.height());
  }

  if (loading_shield_->visible()) {
    return gfx::Size(width + insets.width(),
                     loading_shield_height_ + insets.height());
  }

  int height = 0;
  const int notification_height = notification_area_->GetHeightForWidth(width);
  if (notification_height > notification_area_->GetInsets().height())
    height += notification_height + views::kRelatedControlVerticalSpacing;

  if (scrollable_area_->visible())
    height += get_minimum_size ? kMinimumContentsHeight : scroll_size.height();

  return gfx::Size(width + insets.width(), height + insets.height());
}
