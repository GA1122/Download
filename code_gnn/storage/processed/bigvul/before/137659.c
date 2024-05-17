void PrintPreviewDialogController::AddObservers(WebContents* contents) {
  registrar_.Add(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
                 content::Source<WebContents>(contents));
  registrar_.Add(this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(&contents->GetController()));

  content::Source<content::RenderProcessHost> rph_source(
      contents->GetMainFrame()->GetProcess());
  if (!registrar_.IsRegistered(this,
      content::NOTIFICATION_RENDERER_PROCESS_CLOSED, rph_source)) {
    registrar_.Add(this, content::NOTIFICATION_RENDERER_PROCESS_CLOSED,
                   rph_source);
    host_contents_count_map_[contents->GetMainFrame()->GetProcess()] = 1;
  } else {
    ++host_contents_count_map_[contents->GetMainFrame()->GetProcess()];
  }
}
