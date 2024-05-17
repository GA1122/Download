void TabStrip::UpdateLayoutTypeFromMouseEvent(views::View* source,
                                              const ui::MouseEvent& event) {
  if (!GetAdjustLayout())
    return;


  switch (event.type()) {
    case ui::ET_MOUSE_PRESSED:
      mouse_move_count_ = 0;
      last_mouse_move_time_ = base::TimeTicks();
      SetResetToShrinkOnExit((event.flags() & ui::EF_FROM_TOUCH) == 0);
      if (reset_to_shrink_on_exit_ && touch_layout_.get()) {
        gfx::Point tab_strip_point(event.location());
        views::View::ConvertPointToTarget(source, this, &tab_strip_point);
        Tab* tab = FindTabForEvent(tab_strip_point);
        if (tab && touch_layout_->IsStacked(GetModelIndexOfTab(tab))) {
          SetLayoutType(TAB_STRIP_LAYOUT_SHRINK, true);
          controller_->LayoutTypeMaybeChanged();
        }
      }
      break;

    case ui::ET_MOUSE_MOVED: {
#if defined(USE_ASH)
      SetResetToShrinkOnExit(true);
#else
      gfx::Point location(event.location());
      ConvertPointToTarget(source, this, &location);
      if (location == last_mouse_move_location_)
        return;   
      last_mouse_move_location_ = location;
      if ((event.flags() & ui::EF_FROM_TOUCH) == 0 &&
          (event.flags() & ui::EF_IS_SYNTHESIZED) == 0) {
        if ((base::TimeTicks::Now() - last_mouse_move_time_).InMilliseconds() <
            kMouseMoveTimeMS) {
          if (mouse_move_count_++ == kMouseMoveCountBeforeConsiderReal)
            SetResetToShrinkOnExit(true);
        } else {
          mouse_move_count_ = 1;
          last_mouse_move_time_ = base::TimeTicks::Now();
        }
      } else {
        last_mouse_move_time_ = base::TimeTicks();
      }
#endif
      break;
    }

    case ui::ET_MOUSE_RELEASED: {
      gfx::Point location(event.location());
      ConvertPointToTarget(source, this, &location);
      last_mouse_move_location_ = location;
      mouse_move_count_ = 0;
      last_mouse_move_time_ = base::TimeTicks();
      if ((event.flags() & ui::EF_FROM_TOUCH) == ui::EF_FROM_TOUCH) {
        SetLayoutType(TAB_STRIP_LAYOUT_STACKED, true);
        controller_->LayoutTypeMaybeChanged();
      }
      break;
    }

    default:
      break;
  }
}
