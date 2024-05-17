RenderProcessHost::iterator RenderProcessHost::AllHostsIterator() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return iterator(g_all_hosts.Pointer());
}
