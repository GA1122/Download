void OmniboxViewViews::OnCompositingStarted(ui::Compositor* compositor,
                                            base::TimeTicks start_time) {
  if (latency_histogram_state_ == COMPOSITING_COMMIT)
    latency_histogram_state_ = COMPOSITING_STARTED;
}
