void StatusBubbleGtk::SetFlipHorizontally(bool flip_horizontally) {
  if (flip_horizontally == flip_horizontally_)
    return;

  flip_horizontally_ = flip_horizontally;

  bool ltr = !base::i18n::IsRTL();
  bool on_left = (ltr && !flip_horizontally) || (!ltr && flip_horizontally);

  gtk_alignment_set_padding(GTK_ALIGNMENT(padding_),
      kInternalTopBottomPadding, kInternalTopBottomPadding,
      kInternalLeftRightPadding + (on_left ? 0 : kCornerSize),
      kInternalLeftRightPadding + (on_left ? kCornerSize : 0));
  gtk_util::SetRoundedWindowEdgesAndBorders(
      container_.get(),
      kCornerSize,
      flip_horizontally ?
          gtk_util::ROUNDED_TOP_LEFT :
          gtk_util::ROUNDED_TOP_RIGHT,
      gtk_util::BORDER_TOP |
          (flip_horizontally ? gtk_util::BORDER_LEFT : gtk_util::BORDER_RIGHT));
  gtk_widget_queue_draw(container_.get());
}
