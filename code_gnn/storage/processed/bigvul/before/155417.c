void ChromeContentBrowserClient::OnCookieChange(
    int process_id,
    int routing_id,
    const GURL& url,
    const GURL& first_party_url,
    const net::CanonicalCookie& cookie,
    bool blocked_by_policy) {
  base::RepeatingCallback<content::WebContents*(void)> wc_getter =
      base::BindRepeating(&GetWebContentsFromProcessAndFrameId, process_id,
                          routing_id);
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&TabSpecificContentSettings::CookieChanged, wc_getter, url,
                     first_party_url, cookie, blocked_by_policy));
}
