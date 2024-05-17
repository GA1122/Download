RenderProcessHost* RenderProcessHost::FromID(int render_process_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return g_all_hosts.Get().Lookup(render_process_id);
}
