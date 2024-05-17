void WebViewPlugin::paint(WebCanvas* canvas, const WebRect& rect) {
  gfx::Rect paintRect(rect_.Intersect(rect));
  if (paintRect.IsEmpty())
    return;

  paintRect.Offset(-rect_.x(), -rect_.y());

#if WEBKIT_USING_CG
  CGContextRef context = canvas;
  CGContextTranslateCTM(context, rect_.x(), rect_.y());
  CGContextSaveGState(context);
#elif WEBKIT_USING_SKIA
  skia::PlatformCanvas* platform_canvas = canvas;
  platform_canvas->translate(SkIntToScalar(rect_.x()),
                             SkIntToScalar(rect_.y()));
  platform_canvas->save();
#endif

  web_view_->layout();
  web_view_->paint(canvas, paintRect);

#if WEBKIT_USING_SKIA
  platform_canvas->restore();
#elif WEBKIT_USING_CG
  CGContextRestoreGState(context);
#endif
}
