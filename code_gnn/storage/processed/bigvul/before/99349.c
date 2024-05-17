void ResourceMessageFilter::UpdateHostZoomLevelsOnUIThread(
    const GURL& url,
    int zoom_level) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::UI));
  host_zoom_map_->SetZoomLevel(url, zoom_level);

  for (RenderProcessHost::iterator i(RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance()) {
    RenderProcessHost* render_process_host = i.GetCurrentValue();
    render_process_host->Send(
        new ViewMsg_SetZoomLevelForCurrentURL(url, zoom_level));
  }
}
