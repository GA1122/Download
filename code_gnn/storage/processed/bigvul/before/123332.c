void RenderWidgetHostViewGtk::Paint(const gfx::Rect& damage_rect) {
  TRACE_EVENT0("ui::gtk", "RenderWidgetHostViewGtk::Paint");

  RenderWidgetHostImpl* render_widget_host =
      RenderWidgetHostImpl::From(GetRenderWidgetHost());
  if (render_widget_host->is_accelerated_compositing_active()) {
    host_->ScheduleComposite();
    return;
  }

  GdkWindow* window = gtk_widget_get_window(view_.get());
  DCHECK(!about_to_validate_and_paint_);

  invalid_rect_ = damage_rect;
  about_to_validate_and_paint_ = true;

  bool force_create = !host_->empty();
  BackingStoreGtk* backing_store = static_cast<BackingStoreGtk*>(
      host_->GetBackingStore(force_create));
  about_to_validate_and_paint_ = false;

  gfx::Rect paint_rect = gfx::Rect(0, 0, kMaxWindowWidth, kMaxWindowHeight);
  paint_rect.Intersect(invalid_rect_);

  if (backing_store) {
    if (window) {
      backing_store->XShowRect(gfx::Point(0, 0),
          paint_rect, ui::GetX11WindowFromGtkWidget(view_.get()));
    }
    if (!whiteout_start_time_.is_null()) {
      base::TimeDelta whiteout_duration = base::TimeTicks::Now() -
          whiteout_start_time_;
      UMA_HISTOGRAM_TIMES("MPArch.RWHH_WhiteoutDuration", whiteout_duration);

      whiteout_start_time_ = base::TimeTicks();
    }
    if (!web_contents_switch_paint_time_.is_null()) {
      base::TimeDelta web_contents_switch_paint_duration =
          base::TimeTicks::Now() - web_contents_switch_paint_time_;
      UMA_HISTOGRAM_TIMES("MPArch.RWH_TabSwitchPaintDuration",
          web_contents_switch_paint_duration);
      web_contents_switch_paint_time_ = base::TimeTicks();
    }
  } else {
    if (window)
      gdk_window_clear(window);
    if (whiteout_start_time_.is_null())
      whiteout_start_time_ = base::TimeTicks::Now();
  }
}
