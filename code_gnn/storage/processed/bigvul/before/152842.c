void MetricsWebContentsObserver::OnTimingUpdated(
    content::RenderFrameHost* render_frame_host,
    mojom::PageLoadTimingPtr timing,
    mojom::PageLoadMetadataPtr metadata,
    mojom::PageLoadFeaturesPtr new_features,
    const std::vector<mojom::ResourceDataUpdatePtr>& resources,
    mojom::PageRenderDataPtr render_data) {
  if (GetMainFrame(render_frame_host) != web_contents()->GetMainFrame()) {
    RecordInternalError(ERR_IPC_FROM_WRONG_FRAME);
    return;
  }

  const bool is_main_frame = (render_frame_host->GetParent() == nullptr);
  if (is_main_frame) {
    bool error = false;
    if (!committed_load_) {
      RecordInternalError(ERR_IPC_WITH_NO_RELEVANT_LOAD);
      error = true;
    }

    if (!web_contents()->GetLastCommittedURL().SchemeIsHTTPOrHTTPS()) {
      RecordInternalError(ERR_IPC_FROM_BAD_URL_SCHEME);
      error = true;
    }

    if (error)
      return;
  } else if (!committed_load_) {
    RecordInternalError(ERR_SUBFRAME_IPC_WITH_NO_RELEVANT_LOAD);
  }

  if (committed_load_) {
    committed_load_->metrics_update_dispatcher()->UpdateMetrics(
        render_frame_host, std::move(timing), std::move(metadata),
        std::move(new_features), resources, std::move(render_data));
  }
}
