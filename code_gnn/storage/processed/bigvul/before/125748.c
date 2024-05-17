void TabSpecificContentSettings::IndexedDBAccessed(int render_process_id,
                                                   int render_view_id,
                                                   const GURL& url,
                                                   const string16& description,
                                                   bool blocked_by_policy) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  TabSpecificContentSettings* settings = Get(render_process_id, render_view_id);
  if (settings)
    settings->OnIndexedDBAccessed(url, description, blocked_by_policy);
}
