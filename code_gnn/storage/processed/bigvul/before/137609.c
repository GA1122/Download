void CreateContextMenuDownload(int render_process_id,
                               int render_view_id,
                               const content::ContextMenuParams& params,
                               bool is_link,
                               const std::string& extra_headers,
                               bool granted) {
  if (!granted)
    return;

  content::WebContents* web_contents =
      GetWebContents(render_process_id, render_view_id);
  if (!web_contents)
    return;

  const GURL& url = is_link ? params.link_url : params.src_url;
  const GURL& referring_url =
      params.frame_url.is_empty() ? params.page_url : params.frame_url;
  content::DownloadManager* dlm =
      content::BrowserContext::GetDownloadManager(
          web_contents->GetBrowserContext());
  std::unique_ptr<content::DownloadUrlParameters> dl_params(
      content::DownloadUrlParameters::CreateForWebContentsMainFrame(
          web_contents, url));
  content::Referrer referrer = content::Referrer::SanitizeForRequest(
      url,
      content::Referrer(referring_url.GetAsReferrer(), params.referrer_policy));
  dl_params->set_referrer(referrer);
  if (is_link)
    dl_params->set_referrer_encoding(params.frame_charset);
  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(extra_headers);
  for (net::HttpRequestHeaders::Iterator it(headers); it.GetNext();)
    dl_params->add_request_header(it.name(), it.value());
  if (!is_link && extra_headers.empty())
    dl_params->set_prefer_cache(true);
  dl_params->set_prompt(false);
  dlm->DownloadUrl(std::move(dl_params));
}
