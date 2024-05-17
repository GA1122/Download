void RenderViewImpl::didClearWindowObject(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidClearWindowObject(frame));

  if (enabled_bindings_ & BINDINGS_POLICY_DOM_AUTOMATION) {
    if (!dom_automation_controller_.get())
      dom_automation_controller_.reset(new DomAutomationController());
    dom_automation_controller_->set_message_sender(
        static_cast<RenderView*>(this));
    dom_automation_controller_->set_routing_id(routing_id());
    dom_automation_controller_->BindToJavascript(frame,
                                                 "domAutomationController");
  }

  InjectDoNotTrackBindings(frame);
}
