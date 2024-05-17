void AutofillPopupViewViews::DoUpdateBoundsAndRedrawPopup() {
  gfx::Rect bounds = delegate()->popup_bounds();

  SetSize(bounds.size());

  gfx::Rect clipping_bounds = CalculateClippingBounds();

  int available_vertical_space = clipping_bounds.height() -
                                 (bounds.y() - clipping_bounds.y()) -
                                 kPopupBottomMargin;

  if (available_vertical_space < bounds.height()) {
    const int extra_width =
        scroll_view_ ? scroll_view_->GetScrollBarLayoutWidth() : 0;
    bounds.set_width(bounds.width() + extra_width);
    bounds.set_height(available_vertical_space);
  }

  bounds.Inset(-GetWidget()->GetRootView()->border()->GetInsets());
  GetWidget()->SetBounds(bounds);

  SchedulePaint();
}
