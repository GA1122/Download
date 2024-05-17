void OverscrollControllerAndroid::OnOverscrolled(
    const DidOverscrollParams& params) {
  if (!enabled_)
    return;

  if (refresh_effect_ && (refresh_effect_->IsActive() ||
                          refresh_effect_->IsAwaitingScrollUpdateAck())) {
    return;
  }

  if (glow_effect_ &&
      glow_effect_->OnOverscrolled(
          base::TimeTicks::Now(),
          gfx::ScaleVector2d(params.accumulated_overscroll, dpi_scale_),
          gfx::ScaleVector2d(params.latest_overscroll_delta, dpi_scale_),
          gfx::ScaleVector2d(params.current_fling_velocity, dpi_scale_),
          gfx::ScaleVector2d(
              params.causal_event_viewport_point.OffsetFromOrigin(),
              dpi_scale_))) {
    SetNeedsAnimate();
  }
}
