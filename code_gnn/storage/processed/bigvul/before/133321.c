gfx::Rect PaletteTray::GetAnchorRect(
    views::Widget* anchor_widget,
    views::TrayBubbleView::AnchorType anchor_type,
    views::TrayBubbleView::AnchorAlignment anchor_alignment) const {
  gfx::Rect r =
      GetBubbleAnchorRect(anchor_widget, anchor_type, anchor_alignment);

  if (IsHorizontalAlignment(shelf_alignment()))
    r.Offset(-r.width() + tray_container()->width(), 0);
  else
    r.Offset(0, -r.height() + tray_container()->height());

  return r;
}
