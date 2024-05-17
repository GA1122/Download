void BrowserPluginGuest::RendererUnresponsive(WebContents* source) {
  base::ProcessHandle process_handle =
      web_contents()->GetRenderProcessHost()->GetHandle();
  base::KillProcess(process_handle, RESULT_CODE_HUNG, false);
  RecordAction(UserMetricsAction("BrowserPlugin.Guest.Hung"));
}
