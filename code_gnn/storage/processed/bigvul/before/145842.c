HeadlessDevToolsManagerDelegate::HeadlessDevToolsManagerDelegate(
    base::WeakPtr<HeadlessBrowserImpl> browser)
    : browser_(std::move(browser)) {
  command_map_["Target.createTarget"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::CreateTarget, base::Unretained(this));
  command_map_["Target.closeTarget"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::CloseTarget, base::Unretained(this));
  command_map_["Target.createBrowserContext"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::CreateBrowserContext,
                 base::Unretained(this));
  command_map_["Target.disposeBrowserContext"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::DisposeBrowserContext,
                 base::Unretained(this));
  command_map_["Browser.close"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::Close, base::Unretained(this));
  command_map_["Browser.getWindowForTarget"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::GetWindowForTarget,
                 base::Unretained(this));
  command_map_["Browser.getWindowBounds"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::GetWindowBounds,
                 base::Unretained(this));
  command_map_["Browser.setWindowBounds"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::SetWindowBounds,
                 base::Unretained(this));
  command_map_["HeadlessExperimental.enable"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::EnableHeadlessExperimental,
                 base::Unretained(this));
  command_map_["HeadlessExperimental.disable"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::DisableHeadlessExperimental,
                 base::Unretained(this));

  unhandled_command_map_["Network.emulateNetworkConditions"] =
      base::Bind(&HeadlessDevToolsManagerDelegate::EmulateNetworkConditions,
                 base::Unretained(this));
  unhandled_command_map_["Network.disable"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::NetworkDisable, base::Unretained(this));

  async_command_map_["Page.printToPDF"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::PrintToPDF, base::Unretained(this));
  async_command_map_["HeadlessExperimental.beginFrame"] = base::Bind(
      &HeadlessDevToolsManagerDelegate::BeginFrame, base::Unretained(this));
}
