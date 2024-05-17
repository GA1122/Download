void TabStrip::OnGestureEvent(ui::GestureEvent* event) {
  SetResetToShrinkOnExit(false);
  switch (event->type()) {
    case ui::ET_GESTURE_SCROLL_END:
    case ui::ET_SCROLL_FLING_START:
    case ui::ET_GESTURE_END:
      EndDrag(END_DRAG_COMPLETE);
      if (adjust_layout_) {
        SetLayoutType(TAB_STRIP_LAYOUT_STACKED, true);
        controller_->LayoutTypeMaybeChanged();
      }
      break;

    case ui::ET_GESTURE_LONG_PRESS:
      if (drag_controller_.get())
        drag_controller_->SetMoveBehavior(TabDragController::REORDER);
      break;

    case ui::ET_GESTURE_LONG_TAP: {
      EndDrag(END_DRAG_CANCEL);
      gfx::Point local_point = event->location();
      Tab* tab = FindTabForEvent(local_point);
      if (tab) {
        ConvertPointToScreen(this, &local_point);
        ShowContextMenuForTab(tab, local_point, ui::MENU_SOURCE_TOUCH);
      }
      break;
    }

    case ui::ET_GESTURE_SCROLL_UPDATE:
      ContinueDrag(this, *event);
      break;

    case ui::ET_GESTURE_BEGIN:
      EndDrag(END_DRAG_CANCEL);
      break;

    case ui::ET_GESTURE_TAP: {
      const int active_index = controller_->GetActiveIndex();
      DCHECK_NE(-1, active_index);
      Tab* active_tab = tab_at(active_index);
      TouchUMA::GestureActionType action = TouchUMA::GESTURE_TABNOSWITCH_TAP;
      if (active_tab->tab_activated_with_last_gesture_begin())
        action = TouchUMA::GESTURE_TABSWITCH_TAP;
      TouchUMA::RecordGestureAction(action);
      break;
    }

    default:
      break;
  }
  event->SetHandled();
}
