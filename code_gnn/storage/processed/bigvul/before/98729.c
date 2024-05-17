void WebPluginDelegatePepper::Paint(WebKit::WebCanvas* canvas,
                                    const gfx::Rect& rect) {
  if (nested_delegate_) {
  } else {
    if (!committed_bitmap_.isNull()) {
#if defined(OS_MACOSX)
      SkAutoLockPixels lock(committed_bitmap_);

      scoped_cftyperef<CGDataProviderRef> data_provider(
          CGDataProviderCreateWithData(
              NULL, committed_bitmap_.getAddr32(0, 0),
              committed_bitmap_.rowBytes() * committed_bitmap_.height(), NULL));
      scoped_cftyperef<CGImageRef> image(
          CGImageCreate(
              committed_bitmap_.width(), committed_bitmap_.height(),
              8, 32, committed_bitmap_.rowBytes(),
              mac_util::GetSystemColorSpace(),
              kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host,
              data_provider, NULL, false, kCGRenderingIntentDefault));

      CGContextSaveGState(canvas);
      float window_height =
          static_cast<float>(CGBitmapContextGetHeight(canvas));
      CGContextTranslateCTM(canvas, 0, window_height);
      CGContextScaleCTM(canvas, 1.0, -1.0);

      CGRect bounds;
      bounds.origin.x = window_rect_.origin().x();
      bounds.origin.y = window_height - window_rect_.origin().y() -
          committed_bitmap_.height();
      bounds.size.width = committed_bitmap_.width();
      bounds.size.height = committed_bitmap_.height();

      CGContextDrawImage(canvas, bounds, image);
      CGContextRestoreGState(canvas);
#else
      gfx::Point origin(window_rect_.origin().x(), window_rect_.origin().y());
      canvas->drawBitmap(committed_bitmap_,
                         SkIntToScalar(window_rect_.origin().x()),
                         SkIntToScalar(window_rect_.origin().y()));
#endif
    }
  }
}
