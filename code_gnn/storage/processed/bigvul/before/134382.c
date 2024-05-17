void TabStrip::MaybeStartDrag(
    Tab* tab,
    const ui::LocatedEvent& event,
    const ui::ListSelectionModel& original_selection) {
  if (IsAnimating() || tab->closing() ||
      controller_->HasAvailableDragActions() == 0) {
    return;
  }

  if (IsImmersiveStyle())
    return;

  int model_index = GetModelIndexOfTab(tab);
  if (!IsValidModelIndex(model_index)) {
    CHECK(false);
    return;
  }
  std::vector<Tab*> tabs;
  int size_to_selected = 0;
  int x = tab->GetMirroredXInView(event.x());
  int y = event.y();
  for (int i = 0; i < tab_count(); ++i) {
    Tab* other_tab = tab_at(i);
    if (IsTabSelected(other_tab)) {
      tabs.push_back(other_tab);
      if (other_tab == tab) {
        size_to_selected = GetSizeNeededForTabs(tabs);
        x = size_to_selected - tab->width() + x;
      }
    }
  }
  DCHECK(!tabs.empty());
  DCHECK(std::find(tabs.begin(), tabs.end(), tab) != tabs.end());
  ui::ListSelectionModel selection_model;
  if (!original_selection.IsSelected(model_index))
    selection_model.Copy(original_selection);
  drag_controller_.reset();
  TabDragController::DetachBehavior detach_behavior =
      TabDragController::DETACHABLE;
  TabDragController::MoveBehavior move_behavior =
      TabDragController::REORDER;
  DCHECK(event.type() == ui::ET_MOUSE_PRESSED ||
         event.type() == ui::ET_GESTURE_BEGIN);
  if (touch_layout_.get() &&
      ((event.type() == ui::ET_MOUSE_PRESSED &&
        (((event.flags() & ui::EF_FROM_TOUCH) &&
          static_cast<const ui::MouseEvent&>(event).IsLeftMouseButton()) ||
         (!(event.flags() & ui::EF_FROM_TOUCH) &&
          static_cast<const ui::MouseEvent&>(event).IsControlDown()))) ||
       (event.type() == ui::ET_GESTURE_BEGIN && !event.IsControlDown()))) {
    move_behavior = TabDragController::MOVE_VISIBILE_TABS;
  }

  views::Widget* widget = GetWidget();
#if defined(OS_WIN)
  if (win8::IsSingleWindowMetroMode())
    detach_behavior = TabDragController::NOT_DETACHABLE;
#endif
  if (event.type() == ui::ET_GESTURE_BEGIN)
    widget->SetCapture(this);
  drag_controller_.reset(new TabDragController);
  drag_controller_->Init(
      this, tab, tabs, gfx::Point(x, y), event.x(), selection_model,
      detach_behavior, move_behavior, EventSourceFromEvent(event));
}
