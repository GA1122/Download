void RenderView::didClearWindowObject(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidClearWindowObject(frame));

  GURL frame_url = frame->document().url();
  if (BindingsPolicy::is_web_ui_enabled(enabled_bindings_) &&
      (frame_url.SchemeIs(chrome::kChromeUIScheme) ||
      frame_url.SchemeIs(chrome::kDataScheme))) {
    GetWebUIBindings()->set_message_sender(this);
    GetWebUIBindings()->set_routing_id(routing_id_);
    GetWebUIBindings()->BindToJavascript(frame, "chrome");
  }
}
