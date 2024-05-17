void WebKitTestController::SendTestConfiguration() {
  RenderViewHost* render_view_host =
      main_window_->web_contents()->GetRenderViewHost();
  ShellTestConfiguration params;
  params.current_working_directory = current_working_directory_;
  params.temp_path = temp_path_;
  params.test_url = test_url_;
  params.enable_pixel_dumping = enable_pixel_dumping_;
  params.allow_external_pages =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kAllowExternalPages);
  params.expected_pixel_hash = expected_pixel_hash_;
  params.initial_size = initial_size_;
  render_view_host->Send(new ShellViewMsg_SetTestConfiguration(
      render_view_host->GetRoutingID(), params));
}
