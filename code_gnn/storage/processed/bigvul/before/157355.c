void HTMLMediaElement::CheckViewportIntersectionTimerFired(TimerBase*) {
  bool should_report_root_bounds = true;
  IntersectionGeometry geometry(nullptr, *this, Vector<Length>(),
                                should_report_root_bounds);
  geometry.ComputeGeometry();
  IntRect intersect_rect = geometry.IntersectionIntRect();
  if (current_intersect_rect_ == intersect_rect)
    return;

  current_intersect_rect_ = intersect_rect;
  bool is_mostly_filling_viewport =
      (current_intersect_rect_.Size().Area() >
       kMostlyFillViewportThreshold * geometry.RootIntRect().Size().Area());
  if (mostly_filling_viewport_ == is_mostly_filling_viewport)
    return;

  mostly_filling_viewport_ = is_mostly_filling_viewport;
  if (web_media_player_)
    web_media_player_->BecameDominantVisibleContent(mostly_filling_viewport_);
}
