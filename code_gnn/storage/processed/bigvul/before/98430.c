void ImageView::SetImage(const SkBitmap& bm) {
  image_ = bm;
  SchedulePaint();
}
