void TabStrip::OnMouseEventInTab(views::View* source,
                                 const ui::MouseEvent& event) {
  UpdateLayoutTypeFromMouseEvent(source, event);
}
