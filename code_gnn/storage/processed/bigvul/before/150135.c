  void Next(LayerTreeHostImpl* host_impl) {
    ++frame_;
    posted_ = false;
    switch (frame_) {
      case 2:
        host_impl->PinchGestureBegin();
        host_impl->PinchGestureUpdate(1.5f, gfx::Point(100, 100));
        host_impl->PinchGestureEnd();
        break;
      case 3:
        host_impl->PinchGestureBegin();
        host_impl->PinchGestureUpdate(1.f / 1.5f, gfx::Point(100, 100));
        break;
      case 4:
        playback_allowed_event_.Reset();
        host_impl->PinchGestureEnd();
        break;
      case 5:
        playback_allowed_event_.Signal();
        break;
    }
  }
