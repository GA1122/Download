  void MaybeStartDrag(Tab* tab,
                      int model_index,
                      const ui::LocatedEvent& event,
                      const ui::ListSelectionModel& original_selection) {
    Tabs tabs;
    int x = tab->GetMirroredXInView(event.x());
    int y = event.y();
    for (int i = 0; i < GetTabCount(); ++i) {
      Tab* other_tab = GetTabAt(i);
      if (tab_strip_->IsTabSelected(other_tab)) {
        tabs.push_back(other_tab);
        if (other_tab == tab)
          x += GetSizeNeededForTabs(tabs) - tab->width();
      }
    }
    DCHECK(!tabs.empty());
    DCHECK(base::ContainsValue(tabs, tab));
    ui::ListSelectionModel selection_model;
    if (!original_selection.IsSelected(model_index))
      selection_model = original_selection;
    drag_controller_.reset();
    TabDragController::MoveBehavior move_behavior = TabDragController::REORDER;
    DCHECK(event.type() == ui::ET_MOUSE_PRESSED ||
           event.type() == ui::ET_GESTURE_TAP_DOWN);
    if (tab_strip_->touch_layout_ &&
        ((event.type() == ui::ET_MOUSE_PRESSED &&
          (((event.flags() & ui::EF_FROM_TOUCH) &&
            static_cast<const ui::MouseEvent&>(event).IsLeftMouseButton()) ||
           (!(event.flags() & ui::EF_FROM_TOUCH) &&
            static_cast<const ui::MouseEvent&>(event).IsControlDown()))) ||
         (event.type() == ui::ET_GESTURE_TAP_DOWN && !event.IsControlDown()))) {
      move_behavior = TabDragController::MOVE_VISIBLE_TABS;
    }

    drag_controller_.reset(new TabDragController);
    drag_controller_->Init(this, tab, tabs, gfx::Point(x, y), event.x(),
                           std::move(selection_model), move_behavior,
                           EventSourceFromEvent(event));
  }
