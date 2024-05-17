void RenderViewImpl::ProcessViewLayoutFlags(const CommandLine& command_line) {
  bool enable_viewport =
      command_line.HasSwitch(switches::kEnableViewport);
  bool enable_fixed_layout =
      command_line.HasSwitch(switches::kEnableFixedLayout);
  bool enable_pinch = enable_viewport ||
                      command_line.HasSwitch(switches::kEnablePinch);

  webview()->enableFixedLayoutMode(enable_fixed_layout || enable_viewport);
  webview()->settings()->setFixedElementsLayoutRelativeToFrame(true);
  if (!enable_pinch &&
      webview()->isAcceleratedCompositingActive() &&
      webkit_preferences_.apply_default_device_scale_factor_in_compositor &&
      device_scale_factor_ != 1) {
    webview()->setPageScaleFactorLimits(1, 1);
  }

  if (enable_viewport) {
    webview()->settings()->setViewportEnabled(true);
  } else if (enable_fixed_layout) {
    std::string str =
        command_line.GetSwitchValueASCII(switches::kEnableFixedLayout);
    std::vector<std::string> tokens;
    base::SplitString(str, ',', &tokens);
    if (tokens.size() == 2) {
      int width, height;
      if (base::StringToInt(tokens[0], &width) &&
          base::StringToInt(tokens[1], &height))
        webview()->setFixedLayoutSize(WebSize(width, height));
    }
  }
}
