void AutomationMouseEventProcessor::RenderViewHostDestroyed(
    RenderViewHost* host) {
  InvokeCallback(automation::Error("The render view host was destroyed"));
}
