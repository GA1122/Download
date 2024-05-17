bool ChromeContentBrowserClient::AllowSharedWorker(
    const GURL& worker_url,
    const GURL& main_frame_url,
    const std::string& name,
    const url::Origin& constructor_origin,
    content::BrowserContext* context,
    int render_process_id,
    int render_frame_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  bool allow =
      CookieSettingsFactory::GetForProfile(Profile::FromBrowserContext(context))
          ->IsCookieAccessAllowed(worker_url, main_frame_url);

  TabSpecificContentSettings::SharedWorkerAccessed(
      render_process_id, render_frame_id, worker_url, name, constructor_origin,
      !allow);
  return allow;
}
