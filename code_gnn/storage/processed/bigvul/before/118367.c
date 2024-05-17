void CardUnmaskPromptViews::Layout() {
  gfx::Rect contents_bounds = GetContentsBounds();
  main_contents_->SetBoundsRect(contents_bounds);

  gfx::RectF input_rect = input_row_->GetContentsBounds();
  View::ConvertRectToTarget(input_row_, this, &input_rect);
  input_rect.set_height(contents_bounds.height());
  contents_bounds.Intersect(gfx::ToNearestRect(input_rect));
  progress_overlay_->SetBoundsRect(contents_bounds);
}
