void RenderViewImpl::didClearWindowObject(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidClearWindowObject(frame));

  GURL frame_url = frame->document().url();
  if ((enabled_bindings_ & content::BINDINGS_POLICY_WEB_UI) &&
      (frame_url.SchemeIs(chrome::kChromeUIScheme) ||
      frame_url.SchemeIs(chrome::kDataScheme))) {
    GetWebUIBindings()->BindToJavascript(frame, "chrome");
  }

  if (enabled_bindings_ & content::BINDINGS_POLICY_DOM_AUTOMATION) {
    if (!dom_automation_controller_.get())
      dom_automation_controller_.reset(new DomAutomationController());
    dom_automation_controller_->set_message_sender(
        static_cast<content::RenderView*>(this));
    dom_automation_controller_->set_routing_id(routing_id());
    dom_automation_controller_->BindToJavascript(frame,
                                                 "domAutomationController");
  }
}
