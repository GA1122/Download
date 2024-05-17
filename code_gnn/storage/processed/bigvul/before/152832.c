void MetricsWebContentsObserver::MediaStartedPlaying(
    const content::WebContentsObserver::MediaPlayerInfo& video_type,
    const content::WebContentsObserver::MediaPlayerId& id) {
  if (GetMainFrame(id.render_frame_host) != web_contents()->GetMainFrame()) {
    return;
  }
  if (committed_load_)
    committed_load_->MediaStartedPlaying(
        video_type, id.render_frame_host == web_contents()->GetMainFrame());
}
