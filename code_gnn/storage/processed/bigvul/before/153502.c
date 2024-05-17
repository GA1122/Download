void TabStrip::UpdateStackedLayoutFromMouseEvent(views::View* source,
                                                 const ui::MouseEvent& event) {
  if (!adjust_layout_)
    return;

#if !defined(OS_CHROMEOS)
  constexpr auto kMouseMoveTime = base::TimeDelta::FromMilliseconds(200);
  constexpr int kMouseMoveCountBeforeConsiderReal = 3;
#endif

  switch (event.type()) {
    case ui::ET_MOUSE_PRESSED:
      mouse_move_count_ = 0;
      last_mouse_move_time_ = base::TimeTicks();
      SetResetToShrinkOnExit((event.flags() & ui::EF_FROM_TOUCH) == 0);
      if (reset_to_shrink_on_exit_ && touch_layout_) {
        gfx::Point tab_strip_point(event.location());
        views::View::ConvertPointToTarget(source, this, &tab_strip_point);
        Tab* tab = FindTabForEvent(tab_strip_point);
        if (tab && touch_layout_->IsStacked(GetModelIndexOfTab(tab))) {
          SetStackedLayout(false);
          controller_->StackedLayoutMaybeChanged();
        }
      }
      break;

    case ui::ET_MOUSE_MOVED: {
#if defined(OS_CHROMEOS)
      SetResetToShrinkOnExit(true);
#else
      gfx::Point location(event.location());
      ConvertPointToTarget(source, this, &location);
      if (location == last_mouse_move_location_)
        return;   
      last_mouse_move_location_ = location;
      if ((event.flags() & ui::EF_FROM_TOUCH) ||
          (event.flags() & ui::EF_IS_SYNTHESIZED)) {
        last_mouse_move_time_ = base::TimeTicks();
      } else if ((base::TimeTicks::Now() - last_mouse_move_time_) >=
                 kMouseMoveTime) {
        mouse_move_count_ = 1;
        last_mouse_move_time_ = base::TimeTicks::Now();
      } else if (mouse_move_count_ < kMouseMoveCountBeforeConsiderReal) {
        ++mouse_move_count_;
      } else {
        SetResetToShrinkOnExit(true);
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
        SetStackedLayout(true);
        controller_->StackedLayoutMaybeChanged();
      }
      break;
    }

    default:
      break;
  }
}
