void WebPluginProxy::Paint(const gfx::Rect& rect) {
#if defined(OS_WIN) || defined(OS_LINUX)
  if (!windowless_canvas_.get())
    return;
#elif defined(OS_MACOSX)
  if (!windowless_context_.get())
    return;
#endif

  gfx::Rect offset_rect = rect;
  offset_rect.Offset(delegate_->GetRect().origin());
#if defined(OS_WIN) || defined(OS_LINUX)
  windowless_canvas_->save();

  SkRect sk_rect = { SkIntToScalar(rect.x()),
                     SkIntToScalar(rect.y()),
                     SkIntToScalar(rect.right()),
                     SkIntToScalar(rect.bottom()) };
  windowless_canvas_->clipRect(sk_rect);

  if (background_canvas_.get()) {
    const SkBitmap& background_bitmap =
        background_canvas_->getTopPlatformDevice().accessBitmap(false);
    windowless_canvas_->drawBitmap(background_bitmap, 0, 0);
  } else {
    SkPaint black_fill_paint;
    black_fill_paint.setARGB(0xFF, 0x00, 0x00, 0x00);
    windowless_canvas_->drawPaint(black_fill_paint);
  }

  windowless_canvas_->translate(SkIntToScalar(-delegate_->GetRect().x()),
                                SkIntToScalar(-delegate_->GetRect().y()));

  delegate_->Paint(windowless_canvas_.get(), offset_rect);

  windowless_canvas_->restore();
#elif defined(OS_MACOSX)
  CGContextSaveGState(windowless_context_);
  if (!background_context_.get()) {
    CGContextSetFillColorWithColor(windowless_context_,
                                   CGColorGetConstantColor(kCGColorBlack));
    CGContextFillRect(windowless_context_, rect.ToCGRect());
  } else {
    scoped_cftyperef<CGImageRef> image(
        CGBitmapContextCreateImage(background_context_));
    CGRect source_rect = rect.ToCGRect();
    source_rect.origin.y = CGImageGetHeight(image) - rect.y() - rect.height();
    scoped_cftyperef<CGImageRef> sub_image(
        CGImageCreateWithImageInRect(image, source_rect));
    CGContextDrawImage(windowless_context_, rect.ToCGRect(), sub_image);
  }
  CGContextClipToRect(windowless_context_, rect.ToCGRect());
  delegate_->Paint(windowless_context_, rect);
  CGContextRestoreGState(windowless_context_);
#endif
}
