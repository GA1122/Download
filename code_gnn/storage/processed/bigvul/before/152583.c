  void ReceiveSetTextDirection(RenderWidget* widget,
                               blink::WebTextDirection direction) {
    widget->OnSetTextDirection(direction);
  }
