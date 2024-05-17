 void AutofillPopupBaseView::SetClipPath() {
   SkRect local_bounds = gfx::RectToSkRect(GetLocalBounds());
   SkScalar radius = SkIntToScalar(GetCornerRadius());
  SkPath clip_path;
  clip_path.addRoundRect(local_bounds, radius, radius);
  set_clip_path(clip_path);
}
