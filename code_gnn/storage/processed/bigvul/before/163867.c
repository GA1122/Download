DevToolsWindow* DevToolsWindow::CreateDevToolsWindowForWorker(
    Profile* profile) {
  base::RecordAction(base::UserMetricsAction("DevTools_InspectWorker"));
  return Create(profile, nullptr, kFrontendWorker, std::string(), false, "",
                "");
}
