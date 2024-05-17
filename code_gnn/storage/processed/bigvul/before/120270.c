void LayerTreeHost::SetOverhangBitmap(const SkBitmap& bitmap) {
  DCHECK(bitmap.width() && bitmap.height());
  DCHECK_EQ(bitmap.bytesPerPixel(), 4);

  SkBitmap bitmap_copy;
  if (bitmap.isImmutable()) {
    bitmap_copy = bitmap;
  } else {
    bitmap.copyTo(&bitmap_copy);
    bitmap_copy.setImmutable();
  }

  UIResourceBitmap overhang_bitmap(bitmap_copy);
  overhang_bitmap.SetWrapMode(UIResourceBitmap::REPEAT);
  overhang_ui_resource_ = ScopedUIResource::Create(this, overhang_bitmap);
}
