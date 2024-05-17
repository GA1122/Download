  void SelectWithLongPress(gfx::Point point) {
    RenderWidgetHostViewAndroid* main_view = GetRenderWidgetHostViewAndroid();

    SendTouch(main_view, ui::MotionEvent::Action::DOWN, point);
    DelayBy(base::TimeDelta::FromMilliseconds(2000));
    SendTouch(main_view, ui::MotionEvent::Action::UP, point);
  }
