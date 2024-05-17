void PanelBrowserView::AnimationProgressed(const ui::Animation* animation) {
  gfx::Rect new_bounds = bounds_animator_->CurrentValueBetween(
      animation_start_bounds_, bounds_);

#if defined(OS_WIN) && !defined(USE_AURA)
  bool use_thick_frame = !GetFrameView()->IsShowingTitlebarOnly();
  UpdateWindowAttribute(GWL_STYLE, WS_THICKFRAME, use_thick_frame);
#endif

  ::BrowserView::SetBounds(new_bounds);
}
