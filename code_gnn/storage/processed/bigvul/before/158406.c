  FakeWindowEventDispatcher(aura::WindowTreeHost* host)
      : WindowEventDispatcher(host, true), processed_touch_event_count_(0) {}
