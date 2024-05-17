void BrowserMainLoop::GetCompositingModeReporter(
    viz::mojom::CompositingModeReporterRequest request) {
  bool use_viz =
      base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableViz);
  if (IsUsingMus() || use_viz) {
  } else {
    compositing_mode_reporter_bindings_.AddBinding(
        compositing_mode_reporter_impl_.get(), std::move(request));
  }
}
