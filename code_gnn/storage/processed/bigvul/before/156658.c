double GetFrameDeviceScaleFactor(const ToRenderFrameHost& adapter) {
  return EvalJs(adapter, "window.devicePixelRatio;").ExtractDouble();
}
