void RenderViewImpl::ProcessAcceleratedPinchZoomFlags(
    const CommandLine& command_line) {
  if (!webview()->isAcceleratedCompositingActive())
    return;

  bool enable_viewport = command_line.HasSwitch(switches::kEnableViewport);
  bool enable_pinch = command_line.HasSwitch(switches::kEnablePinch)
      || command_line.HasSwitch(switches::kEnableCssTransformPinch);

  if (enable_viewport)
    return;

  if (enable_pinch)
    webview()->setPageScaleFactorLimits(1, 4);
  else
    webview()->setPageScaleFactorLimits(1, 1);
}
