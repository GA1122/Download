void Browser::Print() {
  UserMetrics::RecordAction(UserMetricsAction("PrintPreview"), profile_);
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnablePrintPreview)) {
    printing::PrintPreviewTabController::PrintPreview(
        GetSelectedTabContents());
  } else {
    GetSelectedTabContentsWrapper()->print_view_manager()->PrintNow();
  }
}
