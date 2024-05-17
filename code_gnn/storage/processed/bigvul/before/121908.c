void NotifyEPMRequestStatus(RequestStatus status,
                            void* profile_id,
                            int process_id,
                            int render_view_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  Profile* profile = reinterpret_cast<Profile*>(profile_id);
  if (!g_browser_process->profile_manager()->IsValidProfile(profile))
    return;

  extensions::ProcessManager* process_manager =
      extensions::ExtensionSystem::Get(profile)->process_manager();
  if (!process_manager)
    return;

  RenderViewHost* render_view_host =
      RenderViewHost::FromID(process_id, render_view_id);
  if (render_view_host) {
    if (status == REQUEST_STARTED) {
      process_manager->OnNetworkRequestStarted(render_view_host);
    } else if (status == REQUEST_DONE) {
      process_manager->OnNetworkRequestDone(render_view_host);
    } else {
      NOTREACHED();
    }
  }
}
