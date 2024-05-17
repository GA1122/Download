  void SimpleTap(gfx::Point point) {
    RenderWidgetHostViewAndroid* main_view = GetRenderWidgetHostViewAndroid();

    SendTouch(main_view, ui::MotionEvent::Action::DOWN, point);
    DelayBy(base::TimeDelta::FromMilliseconds(300));
    SendTouch(main_view, ui::MotionEvent::Action::UP, point);
  }
