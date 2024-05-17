MetricsWebContentsObserver::MetricsWebContentsObserver(
    content::WebContents* web_contents,
    std::unique_ptr<PageLoadMetricsEmbedderInterface> embedder_interface)
    : content::WebContentsObserver(web_contents),
      in_foreground_(web_contents->GetVisibility() !=
                     content::Visibility::HIDDEN),
      embedder_interface_(std::move(embedder_interface)),
      has_navigated_(false),
      page_load_metrics_binding_(web_contents, this) {
  const bool is_prerender =
      prerender::PrerenderContents::FromWebContents(web_contents) != nullptr;
  if (is_prerender)
    in_foreground_ = false;

  RegisterInputEventObserver(web_contents->GetRenderViewHost());
}
