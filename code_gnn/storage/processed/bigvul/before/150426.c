  void SendGestureEvents(aura::Window* window,
                         const gfx::Point& location,
                         bool fling = false,
                         float velocity = 0.f) {
    ash::wm::WindowState* window_state = ash::wm::GetWindowState(window);
    window_state->CreateDragDetails(gfx::Point(0, 0), HTCLIENT,
                                    ::wm::WINDOW_MOVE_SOURCE_TOUCH);
    std::unique_ptr<ash::TabletModeWindowDragController> controller_ =
        std::make_unique<ash::TabletModeWindowDragController>(
            window_state,
            std::make_unique<ash::TabletModeBrowserWindowDragDelegate>());
    controller_->drag_delegate_for_testing()
        ->set_drag_start_deadline_for_testing(base::Time::Now());
    controller_->Drag(location, 0);
    if (fling) {
      ui::GestureEventDetails details =
          ui::GestureEventDetails(ui::ET_SCROLL_FLING_START, 0, velocity);
      ui::GestureEvent event =
          ui::GestureEvent(location.x(), location.y(), ui::EF_NONE,
                           base::TimeTicks::Now(), details);
      ui::Event::DispatcherApi(&event).set_target(window);
      controller_->FlingOrSwipe(&event);
    } else {
      controller_->CompleteDrag();
    }
    ash::wm::GetWindowState(window)->DeleteDragDetails();
  }
