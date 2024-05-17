void AutofillDialogViews::OverlayView::Layout() {
  gfx::Rect bounds = ContentBoundsSansBubbleBorder();
  if (!message_view_->visible()) {
    image_view_->SetBoundsRect(bounds);
    return;
  }

  int message_height = message_view_->GetHeightForWidth(bounds.width());
  int y = bounds.bottom() - message_height;
  message_view_->SetBounds(bounds.x(), y, bounds.width(), message_height);

  gfx::Size image_size = image_view_->GetPreferredSize();
  y -= image_size.height() + kOverlayImageBottomMargin;
  image_view_->SetBounds(bounds.x(), y, bounds.width(), image_size.height());
}
