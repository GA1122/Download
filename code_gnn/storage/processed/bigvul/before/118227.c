int AutofillDialogViews::OverlayView::GetHeightForContentsForWidth(int width) {
  if (!message_view_->visible())
    return 0;

  return kOverlayImageBottomMargin +
      views::kButtonVEdgeMarginNew +
      message_view_->GetHeightForWidth(width) +
      image_view_->GetHeightForWidth(width);
}
