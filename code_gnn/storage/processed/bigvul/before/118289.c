void AutofillDialogViews::NotificationArea::PaintChildren(
    const PaintContext& context) {
  views::View::PaintChildren(context);
  gfx::Canvas* canvas = context.canvas();
  if (HasArrow()) {
    DrawArrow(
        canvas,
        GetMirroredXInView(width() - arrow_centering_anchor_->width() / 2.0f),
        notifications_[0].GetBackgroundColor(),
        notifications_[0].GetBorderColor());
  }
}
