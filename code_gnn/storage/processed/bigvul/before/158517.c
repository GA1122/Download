void WebLocalFrameImpl::CreateFrameView() {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::createFrameView");

  DCHECK(GetFrame());   

  WebViewImpl* web_view = ViewImpl();

  if (!web_view->GetPage())
    return;

  bool is_main_frame = !Parent();
  IntSize initial_size = (is_main_frame || !frame_widget_)
                             ? web_view->MainFrameSize()
                             : static_cast<IntSize>(frame_widget_->Size());
  Color base_background_color = web_view->BaseBackgroundColor();
  if (!is_main_frame && Parent()->IsWebRemoteFrame())
    base_background_color = Color::kTransparent;

  GetFrame()->CreateView(initial_size, base_background_color);
  if (is_main_frame) {
    GetFrame()->View()->SetInitialViewportSize(
        web_view->GetPageScaleConstraintsSet().InitialViewportSize());
  }
  if (web_view->ShouldAutoResize() && GetFrame()->IsLocalRoot()) {
    GetFrame()->View()->EnableAutoSizeMode(web_view->MinAutoSize(),
                                           web_view->MaxAutoSize());
  }

  GetFrame()->View()->SetInputEventsScaleForEmulation(
      input_events_scale_factor_for_emulation_);
  GetFrame()->View()->SetDisplayMode(web_view->DisplayMode());

  if (frame_widget_)
    frame_widget_->DidCreateLocalRootView();
}
