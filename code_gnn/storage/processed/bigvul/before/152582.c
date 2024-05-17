  void ReceiveEnableDeviceEmulation(
      RenderViewImpl* view,
      const blink::WebDeviceEmulationParams& params) {
    view->GetWidget()->OnEnableDeviceEmulation(params);
  }
