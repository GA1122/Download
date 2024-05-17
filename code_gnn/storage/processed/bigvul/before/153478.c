bool TabStrip::ShouldTabBeVisible(const Tab* tab) const {
  if (tab->detached())
    return false;

  if (stacked_layout_)
    return true;

  const int right_edge = tab->bounds().right();
  const int tabstrip_right =
      tab->dragging() ? drag_context_->TabDragAreaEndX() : GetTabAreaWidth();
  if (right_edge > tabstrip_right)
    return false;

  if (tab->dragging())
    return true;

  if (tab->closing())
    return true;


  if (tab->data().pinned)
    return true;

  if (controller_->GetActiveIndex() <= GetModelIndexOfTab(tab))
    return true;

  return (right_edge + GetActiveTabWidth() - GetInactiveTabWidth()) <=
         tabstrip_right;
}
