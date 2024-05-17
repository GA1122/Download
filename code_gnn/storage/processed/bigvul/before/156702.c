  void SendTouch(RenderWidgetHostViewAndroid* view,
                 ui::MotionEvent::Action action,
                 gfx::Point point) {
    DCHECK(action >= ui::MotionEvent::Action::DOWN &&
           action < ui::MotionEvent::Action::CANCEL);

    ui::MotionEventAndroid::Pointer p(0, point.x(), point.y(), 10, 0, 0, 0, 0);
    JNIEnv* env = base::android::AttachCurrentThread();
    auto time_ms = (ui::EventTimeForNow() - base::TimeTicks()).InMilliseconds();
    ui::MotionEventAndroid touch(
        env, nullptr, 1.f, 0, 0, 0, time_ms,
        ui::MotionEventAndroid::GetAndroidAction(action), 1, 0, 0, 0, 0, 0, 0,
        0, false, &p, nullptr);
    view->OnTouchEvent(touch);
  }
