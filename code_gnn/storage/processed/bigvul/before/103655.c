bool ChromeContentRendererClient::ShouldPumpEventsDuringCookieMessage() {
  return CommandLine::ForCurrentProcess()->HasSwitch(switches::kChromeFrame);
}
