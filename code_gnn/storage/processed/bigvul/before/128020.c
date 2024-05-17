bool AwContents::OnDraw(JNIEnv* env,
                        jobject obj,
                        jobject canvas,
                        jboolean is_hardware_accelerated,
                        jint scroll_x,
                        jint scroll_y,
                        jint visible_left,
                        jint visible_top,
                        jint visible_right,
                        jint visible_bottom) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  gfx::Vector2d scroll(scroll_x, scroll_y);
  browser_view_renderer_.PrepareToDraw(
      scroll, gfx::Rect(visible_left, visible_top, visible_right - visible_left,
                        visible_bottom - visible_top));
  if (is_hardware_accelerated && browser_view_renderer_.attached_to_window() &&
      !g_force_auxiliary_bitmap_rendering) {
    return browser_view_renderer_.OnDrawHardware();
  }

  gfx::Size view_size = browser_view_renderer_.size();
  if (view_size.IsEmpty()) {
    TRACE_EVENT_INSTANT0("android_webview", "EarlyOut_EmptySize",
                         TRACE_EVENT_SCOPE_THREAD);
    return false;
  }

  scoped_ptr<SoftwareCanvasHolder> canvas_holder = SoftwareCanvasHolder::Create(
      canvas, scroll, view_size, g_force_auxiliary_bitmap_rendering);
  if (!canvas_holder || !canvas_holder->GetCanvas()) {
    TRACE_EVENT_INSTANT0("android_webview", "EarlyOut_NoSoftwareCanvas",
                         TRACE_EVENT_SCOPE_THREAD);
    return false;
  }
  return browser_view_renderer_.OnDrawSoftware(canvas_holder->GetCanvas());
}
