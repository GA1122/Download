void WebPluginProxy::InvalidateRect(const gfx::Rect& rect) {
#if defined(OS_MACOSX)
  gfx::Rect plugin_rect = delegate_->GetRect();
  plugin_rect.set_origin(gfx::Point(0, 0));
  const gfx::Rect invalidate_rect(rect.Intersect(plugin_rect));
#else
  const gfx::Rect invalidate_rect(rect);
#endif
  damaged_rect_ = damaged_rect_.Union(invalidate_rect);
  if (invalidate_rect.IsEmpty() ||
      !delegate_->GetClipRect().Intersects(invalidate_rect))
    return;

  if (!waiting_for_paint_) {
    waiting_for_paint_ = true;
    MessageLoop::current()->PostTask(FROM_HERE,
        runnable_method_factory_.NewRunnableMethod(
            &WebPluginProxy::OnPaint, damaged_rect_));
    damaged_rect_ = gfx::Rect();
  }
}
