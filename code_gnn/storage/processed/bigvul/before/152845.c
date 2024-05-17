void MetricsWebContentsObserver::RecordFeatureUsage(
    content::RenderFrameHost* render_frame_host,
    const mojom::PageLoadFeatures& new_features) {
  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(render_frame_host);
  MetricsWebContentsObserver* observer =
      MetricsWebContentsObserver::FromWebContents(web_contents);
  if (observer)
    observer->OnBrowserFeatureUsage(render_frame_host, new_features);
}
