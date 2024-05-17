void ChromeContentBrowserClient::OnCookiesRead(
    int process_id,
    int routing_id,
    const GURL& url,
    const GURL& first_party_url,
    const net::CookieList& cookie_list,
    bool blocked_by_policy) {
  base::RepeatingCallback<content::WebContents*(void)> wc_getter =
      base::BindRepeating(&GetWebContentsFromProcessAndFrameId, process_id,
                          routing_id);
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&TabSpecificContentSettings::CookiesRead, wc_getter, url,
                     first_party_url, cookie_list, blocked_by_policy));
}
