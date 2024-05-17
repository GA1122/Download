void ImageView::SetImage(SkBitmap* bm) {
  if (bm) {
    SetImage(*bm);
  } else {
    SkBitmap t;
    SetImage(t);
  }
}
