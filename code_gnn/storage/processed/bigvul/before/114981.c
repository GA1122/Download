void TestingAutomationProvider::LoadBlockedPlugins(int tab_handle,
                                                   bool* success) {
  *success = false;
  if (tab_tracker_->ContainsHandle(tab_handle)) {
    NavigationController* nav = tab_tracker_->GetResource(tab_handle);
    if (!nav)
      return;
    WebContents* contents = nav->GetWebContents();
    if (!contents)
      return;
    RenderViewHost* host = contents->GetRenderViewHost();
    host->Send(new ChromeViewMsg_LoadBlockedPlugins(host->routing_id()));
    *success = true;
  }
}
