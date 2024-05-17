void RenderWidget::PaintRect(const gfx::Rect& rect,
                             const gfx::Point& canvas_origin,
                             skia::PlatformCanvas* canvas) {
  canvas->save();

  canvas->translate(static_cast<SkScalar>(-canvas_origin.x()),
                    static_cast<SkScalar>(-canvas_origin.y()));

  if (!background_.empty()) {
    SkPaint paint;
    SkShader* shader = SkShader::CreateBitmapShader(background_,
                                                    SkShader::kRepeat_TileMode,
                                                    SkShader::kRepeat_TileMode);
    paint.setShader(shader)->unref();
    paint.setXfermodeMode(SkXfermode::kSrcOver_Mode);
    canvas->drawPaint(paint);
  }

  webwidget_->paint(webkit_glue::ToWebCanvas(canvas), rect);

  PaintDebugBorder(rect, canvas);

  canvas->getTopPlatformDevice().accessBitmap(false);

  canvas->restore();
}
