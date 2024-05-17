void WebPluginDelegateProxy::PaintSadPlugin(WebKit::WebCanvas* native_context,
                                            const gfx::Rect& rect) {
  if (!sad_plugin_) {
    sad_plugin_ = ResourceBundle::GetSharedInstance().GetBitmapNamed(
        IDR_SAD_PLUGIN);
  }
  if (!sad_plugin_)
    return;

  const int width = plugin_rect_.width();
  const int height = plugin_rect_.height();
  gfx::Canvas canvas(width, height, false);
#if defined(OS_MACOSX)
  canvas.translate(0, height);
  canvas.scale(1, -1);
#endif
  SkPaint paint;

  paint.setStyle(SkPaint::kFill_Style);
  paint.setColor(SK_ColorBLACK);
  canvas.drawRectCoords(0, 0, SkIntToScalar(width), SkIntToScalar(height),
                        paint);
  canvas.DrawBitmapInt(*sad_plugin_,
                       std::max(0, (width - sad_plugin_->width())/2),
                       std::max(0, (height - sad_plugin_->height())/2));

#if WEBKIT_USING_SKIA
  gfx::NativeDrawingContext context = native_context->beginPlatformPaint();
  BlitCanvasToContext(context, plugin_rect_, &canvas, gfx::Point(0, 0));
  native_context->endPlatformPaint();
#elif WEBKIT_USING_CG
  BlitCanvasToContext(native_context, plugin_rect_, &canvas, gfx::Point(0, 0));
#endif
}
