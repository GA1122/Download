void TabSpecificContentSettings::DOMStorageAccessed(int render_process_id,
                                                    int render_view_id,
                                                    const GURL& url,
                                                    bool local,
                                                    bool blocked_by_policy) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  TabSpecificContentSettings* settings = Get(render_process_id, render_view_id);
  if (settings)
    settings->OnLocalStorageAccessed(url, local, blocked_by_policy);
}
