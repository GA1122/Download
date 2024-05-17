void RenderViewImpl::PageImportanceSignalsChanged() {
  if (!webview() || !main_render_frame_)
    return;

  auto* web_signals = webview()->PageImportanceSignals();

  PageImportanceSignals signals;
  signals.had_form_interaction = web_signals->HadFormInteraction();

  main_render_frame_->Send(new FrameHostMsg_UpdatePageImportanceSignals(
      main_render_frame_->GetRoutingID(), signals));
}
