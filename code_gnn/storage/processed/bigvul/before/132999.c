void RenderWidgetHostViewAura::OnUpdateVSyncParameters(
    ui::Compositor* compositor,
    base::TimeTicks timebase,
    base::TimeDelta interval) {
  if (IsShowing()) {
    if (IsDeadlineSchedulingEnabled()) {
      host_->UpdateVSyncParameters(timebase, interval);
    } else if (!last_draw_ended_.is_null()) {
      host_->UpdateVSyncParameters(last_draw_ended_, interval);
    }
  }
}
