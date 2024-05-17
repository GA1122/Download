void OmniboxViewViews::OnCompositingDidCommit(ui::Compositor* compositor) {
  if (latency_histogram_state_ == ON_PAINT_CALLED) {
    latency_histogram_state_ = COMPOSITING_COMMIT;
  } else if (latency_histogram_state_ == COMPOSITING_COMMIT) {
    insert_char_time_ = base::TimeTicks();
    latency_histogram_state_ = NOT_ACTIVE;
  }
}
