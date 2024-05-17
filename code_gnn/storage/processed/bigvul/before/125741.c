void TabSpecificContentSettings::CookiesRead(int render_process_id,
                                             int render_view_id,
                                             const GURL& url,
                                             const GURL& frame_url,
                                             const net::CookieList& cookie_list,
                                             bool blocked_by_policy) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  TabSpecificContentSettings* settings = Get(render_process_id, render_view_id);
  if (settings) {
    settings->OnCookiesRead(url, frame_url, cookie_list,
                            blocked_by_policy);
  }
}
