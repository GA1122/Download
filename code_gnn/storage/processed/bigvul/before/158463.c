  void SimulateMouseEvent(WebInputEvent::Type type) {
    widget_host_->ForwardMouseEvent(SyntheticWebMouseEventBuilder::Build(type));
    base::RunLoop().RunUntilIdle();
  }
