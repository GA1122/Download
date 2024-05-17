void RenderViewImpl::ProcessViewLayoutFlags(const CommandLine& command_line) {
  bool enable_viewport =
      command_line.HasSwitch(switches::kEnableViewport);
  bool enable_fixed_layout =
      command_line.HasSwitch(switches::kEnableFixedLayout);

  webview()->enableFixedLayoutMode(enable_fixed_layout || enable_viewport);
  webview()->settings()->setFixedElementsLayoutRelativeToFrame(true);

  if (enable_viewport)
    return;

  if (enable_fixed_layout) {
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

  if (command_line.HasSwitch(switches::kEnablePinch))
    webview()->setPageScaleFactorLimits(1, 4);
  else
    webview()->setPageScaleFactorLimits(1, 1);
}
