void WebPluginProxy::Paint(const gfx::Rect& rect) {
#if defined(OS_MACOSX)
  if (!windowless_context())
    return;
#else
  if (!windowless_canvas() || !windowless_canvas()->getDevice())
    return;
#endif

  gfx::Rect offset_rect = rect;
  offset_rect.Offset(delegate_->GetRect().origin());
#if defined(OS_MACOSX)
  CGContextSaveGState(windowless_context());
  CGContextRef saved_context_weak = windowless_context();
  int saved_index = windowless_buffer_index_;

  if (background_context_.get()) {
    base::mac::ScopedCFTypeRef<CGImageRef> image(
        CGBitmapContextCreateImage(background_context_));
    CGRect source_rect = rect.ToCGRect();
    source_rect.origin.y = CGImageGetHeight(image) - rect.y() - rect.height();
    base::mac::ScopedCFTypeRef<CGImageRef> sub_image(
        CGImageCreateWithImageInRect(image, source_rect));
    CGContextDrawImage(windowless_context(), rect.ToCGRect(), sub_image);
  } else if (transparent_) {
    CGContextClearRect(windowless_context(), rect.ToCGRect());
  }
  CGContextClipToRect(windowless_context(), rect.ToCGRect());
  delegate_->CGPaint(windowless_context(), rect);
  if (windowless_contexts_[saved_index].get() == saved_context_weak)
    CGContextRestoreGState(windowless_contexts_[saved_index]);
#else
  windowless_canvas()->save();

  SkRect sk_rect = { SkIntToScalar(rect.x()),
                     SkIntToScalar(rect.y()),
                     SkIntToScalar(rect.right()),
                     SkIntToScalar(rect.bottom()) };
  windowless_canvas()->clipRect(sk_rect);

  if (background_canvas_.get() && background_canvas_.get()->getDevice()) {
    const SkBitmap& background_bitmap =
        skia::GetTopDevice(*background_canvas_)->accessBitmap(false);
    windowless_canvas()->drawBitmap(background_bitmap, 0, 0);
  } else {
    SkPaint black_fill_paint;
    black_fill_paint.setARGB(0xFF, 0x00, 0x00, 0x00);
    windowless_canvas()->drawPaint(black_fill_paint);
  }

  windowless_canvas()->translate(SkIntToScalar(-delegate_->GetRect().x()),
                                 SkIntToScalar(-delegate_->GetRect().y()));

  delegate_->Paint(windowless_canvas(), offset_rect);

  windowless_canvas()->restore();
#endif
}
