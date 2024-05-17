void RenderWidgetHostViewAura::SetIsLoading(bool is_loading) {
  if (is_loading_ && !is_loading && paint_observer_)
    paint_observer_->OnPageLoadComplete();
  is_loading_ = is_loading;
  UpdateCursorIfOverSelf();
}
