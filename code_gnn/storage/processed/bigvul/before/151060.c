DevToolsWindow* DevToolsWindow::CreateDevToolsWindowForWorker(
    Profile* profile) {
  content::RecordAction(base::UserMetricsAction("DevTools_InspectWorker"));
  return Create(profile, GURL(), NULL, true, false, std::string(), false, "",
                "");
}
