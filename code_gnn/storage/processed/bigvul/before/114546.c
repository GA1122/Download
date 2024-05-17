void WebPluginProxy::InvalidateRect(const gfx::Rect& rect) {
#if defined(OS_MACOSX)
  if (!windowless_context()) {
    delegate_->PluginDidInvalidate();
    return;
  }

  gfx::Rect plugin_rect = delegate_->GetRect();
  plugin_rect.set_origin(gfx::Point(0, 0));
  const gfx::Rect invalidate_rect(rect.Intersect(plugin_rect));
#else
  const gfx::Rect invalidate_rect(rect);
#endif
  damaged_rect_ = damaged_rect_.Union(invalidate_rect);
  if (damaged_rect_.IsEmpty() ||
      !delegate_->GetClipRect().Intersects(damaged_rect_))
    return;

  if (!waiting_for_paint_) {
    waiting_for_paint_ = true;
    MessageLoop::current()->PostTask(FROM_HERE,
        base::Bind(&WebPluginProxy::OnPaint, weak_factory_.GetWeakPtr(),
            damaged_rect_));
    damaged_rect_ = gfx::Rect();
  }
}
