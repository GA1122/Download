void Browser::SetContentsBounds(WebContents* source, const gfx::Rect& bounds) {
  if (!is_type_popup())
    return;

  window_->SetBounds(bounds);
}
