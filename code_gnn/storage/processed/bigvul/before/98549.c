void WmOverviewSnapshot::SetImage(const SkBitmap& image) {
  CHECK(snapshot_view_) << "Init not called before setting image.";
  snapshot_view_->SetImage(image);

  gfx::Rect bounds;
  GetBounds(&bounds, false);
  bounds.set_width(image.width());
  bounds.set_height(image.height());
  SetBounds(bounds);

  configured_snapshot_ = true;
}
