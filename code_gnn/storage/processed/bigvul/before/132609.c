void WebKitTestController::TestFinishedInSecondaryWindow() {
  RenderViewHost* render_view_host =
      main_window_->web_contents()->GetRenderViewHost();
  render_view_host->Send(
      new ShellViewMsg_NotifyDone(render_view_host->GetRoutingID()));
}
