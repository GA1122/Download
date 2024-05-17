void TestingAutomationProvider::GetTabProcessID(int handle, int* process_id) {
  *process_id = -1;

  if (tab_tracker_->ContainsHandle(handle)) {
    *process_id = 0;
    WebContents* web_contents =
        tab_tracker_->GetResource(handle)->GetWebContents();
    content::RenderProcessHost* rph = web_contents->GetRenderProcessHost();
    if (rph)
      *process_id = base::GetProcId(rph->GetHandle());
  }
}
