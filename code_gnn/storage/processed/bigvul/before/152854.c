void MetricsWebContentsObserver::UpdateTiming(
    mojom::PageLoadTimingPtr timing,
    mojom::PageLoadMetadataPtr metadata,
    mojom::PageLoadFeaturesPtr new_features,
    std::vector<mojom::ResourceDataUpdatePtr> resources,
    mojom::PageRenderDataPtr render_data) {
  content::RenderFrameHost* render_frame_host =
      page_load_metrics_binding_.GetCurrentTargetFrame();
  OnTimingUpdated(render_frame_host, std::move(timing), std::move(metadata),
                  std::move(new_features), resources, std::move(render_data));
}
