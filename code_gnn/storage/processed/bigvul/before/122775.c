void BrowserPluginGuest::Terminate() {
  RecordAction(UserMetricsAction("BrowserPlugin.Guest.Terminate"));
  base::ProcessHandle process_handle =
      web_contents()->GetRenderProcessHost()->GetHandle();
  base::KillProcess(process_handle, RESULT_CODE_KILLED, false);
}
