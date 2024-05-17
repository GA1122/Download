void TabSpecificContentSettings::WebDatabaseAccessed(
    int render_process_id,
    int render_view_id,
    const GURL& url,
    const string16& name,
    const string16& display_name,
    bool blocked_by_policy) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  TabSpecificContentSettings* settings = Get(render_process_id, render_view_id);
  if (settings)
    settings->OnWebDatabaseAccessed(url, name, display_name, blocked_by_policy);
}
